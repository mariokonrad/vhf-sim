// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "MainWindow.hpp"
#include <QMenuBar>
#include <QCoreApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTcpSocket>
#include <QProgressDialog>
#include <QDebug>
#include "version.hpp"
#include "Widget.hpp"
#include "System.hpp"
#include "vhfpreferences.hpp"
#include "connectionpreferences.hpp"
#include "gpswindow.hpp"
#include "controlcenter.hpp"
#include "MsgSenderControlCenter.hpp"
#include "MsgSenderSocket.hpp"
#include "etherwindow.hpp"

namespace simradrd68
{
MainWindow::MainWindow()
	: socket(nullptr)
	, action_open_connection(nullptr)
	, action_close_connection(nullptr)
	, action_control_center(nullptr)
{
	setWindowIcon(QIcon(":icons/vhf-main.xpm"));
	set_title();

	// main widget

	widget = new Widget(this);
	setCentralWidget(widget);

	// GPS frame

	gps = new GPSWindow(this);

	// this connection has to be a lambda, because gps_process is not a slot
	connect(gps, &GPSWindow::sentence,
		[this](const std::string & s) { this->widget->gps_process(s); });

	// controlcenter

	controlcenter = new ControlCenter(this);
	connect(controlcenter, &ControlCenter::send,
		[this](const engine::msg_t & m) { this->widget->process(m); });

	// ether

	ether = new EtherWindow(this);

	// actions

	auto action_about = new QAction(tr("About"), this);
	action_about->setStatusTip(project_name);
	connect(action_about, &QAction::triggered, this, &MainWindow::on_about);

	auto action_about_qt = new QAction(tr("About Qt"), this);
	action_about_qt->setStatusTip(tr("Shows information about Qt"));
	connect(action_about_qt, &QAction::triggered, this, &MainWindow::on_about_qt);

	auto action_exit_application = new QAction(tr("E&xit"), this);
	action_exit_application->setStatusTip(tr("Closes the application"));
	action_exit_application->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
	connect(action_exit_application, &QAction::triggered, this, &MainWindow::close);

	auto action_toggle_fullscreen = new QAction(tr("Fullscreen"), this);
	action_toggle_fullscreen->setStatusTip(tr("Shows the window in fullscreen"));
	action_toggle_fullscreen->setCheckable(true);
	action_toggle_fullscreen->setChecked(false);
	action_toggle_fullscreen->setShortcut(QKeySequence(Qt::Key_F11));
	connect(
		action_toggle_fullscreen, &QAction::toggled, this, &MainWindow::on_toggle_fullscreen);

	auto action_show_buttons = new QAction(tr("Show Buttons"), this);
	action_show_buttons->setCheckable(true);
	action_show_buttons->setChecked(false);
	connect(action_show_buttons, &QAction::toggled, this, &MainWindow::on_show_buttons);

	action_open_connection = new QAction(tr("Open Connection..."), this);
	action_open_connection->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
	connect(action_open_connection, &QAction::triggered, this, &MainWindow::on_connection_open);

	action_close_connection = new QAction(tr("Close Connection"), this);
	connect(
		action_close_connection, &QAction::triggered, this, &MainWindow::on_connection_close);

	action_control_center = new QAction(tr("Control Center..."), this);
	action_control_center->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	connect(action_control_center, &QAction::triggered, this, &MainWindow::on_controlcenter);

	auto action_show_gps = new QAction(tr("GPS..."), this);
	action_show_gps->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	connect(action_show_gps, &QAction::triggered, this, &MainWindow::on_show_gps);

	auto action_vhf_preferences = new QAction(tr("VHF Preferences..."), this);
	action_vhf_preferences->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	connect(action_vhf_preferences, &QAction::triggered, this, &MainWindow::on_vhf_preferences);

	auto action_connection_preferences = new QAction(tr("Connection Preferences..."), this);
	action_connection_preferences->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
	connect(action_connection_preferences, &QAction::triggered, this,
		&MainWindow::on_connection_preferences);

	auto action_exam_mode = new QAction(tr("Exam Mode"), this);
	action_exam_mode->setCheckable(true);
	action_exam_mode->setChecked(System::exam_mode());
	connect(action_exam_mode, &QAction::toggled, this, &MainWindow::on_exam_mode);

	auto action_comm_hub = new QAction(tr("Communication Hub..."), this);
	connect(action_comm_hub, &QAction::triggered, this, &MainWindow::on_comm_hub);

	// menubar

	auto menubar = menuBar();

	auto menu_file = menubar->addMenu(tr("&File"));
	menu_file->addAction(action_show_gps);
	menu_file->addAction(action_vhf_preferences);
	menu_file->addSeparator();
	menu_file->addAction(action_exam_mode);
	menu_file->addSeparator();
	menu_file->addAction(action_exit_application);

	auto menu_comm = menubar->addMenu(tr("&Communication"));
	menu_comm->addAction(action_control_center);
	menu_comm->addSeparator();
	menu_comm->addAction(action_connection_preferences);
	menu_comm->addAction(action_open_connection);
	menu_comm->addAction(action_close_connection);
	menu_comm->addAction(action_comm_hub);

	auto menu_view = menubar->addMenu(tr("&View"));
	menu_view->addAction(action_toggle_fullscreen);
	menu_view->addAction(action_show_buttons);

	auto menu_help = menubar->addMenu(tr("&Help"));
	menu_help->addAction(action_about);
	menu_help->addAction(action_about_qt);

	// misc
	connect_to_controlcenter();
	handle_menu_entries();
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent * event)
{
	gps->hide();
	event->accept();
}

void MainWindow::set_title()
{
	QString title = QCoreApplication::instance()->applicationName();

	if (System::exam_mode())
		title += tr(" (Exam Mode)");
	if (System::sound_disabled())
		title += tr(" (Audio Off)");

	setWindowTitle(title);
}

void MainWindow::on_toggle_fullscreen(bool checked)
{
	if (checked) {
		showFullScreen();
	} else {
		showNormal();
	}
}

void MainWindow::on_show_buttons(bool checked) { widget->show_buttons(checked); }

void MainWindow::on_connection_open()
{
	// close open socket (this should not happen, but it does not hurt)
	socket_close();
	socket = new QTcpSocket(this);

	const QString host = System::com_host();
	const int port = System::com_port();

	constexpr int max_tries = 15;

	bool is_connected = false;
	socket->connectToHost(host, port);
	QProgressDialog progress(this);
	progress.setMinimumDuration(100);
	progress.setRange(0, max_tries);
	for (int i = 0; !is_connected && (i < max_tries); ++i) {
		is_connected = socket->waitForConnected(1000);
		progress.setValue(i);
	}

	if (is_connected) {
		// set socket as the current connection, discard the control center
		widget->set_msg_sender(std::unique_ptr<MsgSender>(new MsgSenderSocket(socket)));
		connect(socket, &QTcpSocket::readyRead, [this]() { this->data_ready(); });

		controlcenter->hide();
	} else {
		QMessageBox::critical(
			this, tr("Connection Error"), tr("Unable to connect to %1:%2").arg(host).arg(port));
		socket_close();
	}

	handle_menu_entries();
}

void MainWindow::data_ready()
{
	using namespace engine;

	msg_t msg;
	auto rc = socket->read(reinterpret_cast<char *>(&msg), sizeof(msg));
	if (rc != sizeof(msg)) {
		return;
	}
	msg = ntoh(msg);

	if (msg.type != engine::MSG_VHF) // ignore all uninteresing messages
		return;

	widget->process(msg);
}

void MainWindow::on_connection_close()
{
	// restore connection to controlcenter, this is the default
	socket_close();
	connect_to_controlcenter();
	handle_menu_entries();
}

void MainWindow::socket_close()
{
	delete socket;
	socket = nullptr;
}

void MainWindow::handle_menu_entries()
{
	if (socket) {
		action_open_connection->setEnabled(false);
		action_close_connection->setEnabled(true);
		action_control_center->setEnabled(false);
	} else {
		action_open_connection->setEnabled(true);
		action_close_connection->setEnabled(false);
		action_control_center->setEnabled(true);
	}
}

void MainWindow::connect_to_controlcenter()
{
	widget->set_msg_sender(
		std::unique_ptr<MsgSender>(new MsgSenderControlCenter(controlcenter)));
}

void MainWindow::on_controlcenter() { controlcenter->show(); }

void MainWindow::on_show_gps() { gps->show(); }

void MainWindow::on_vhf_preferences()
{
	static const std::vector<std::pair<QString, QString>> languages = {
		{tr("English"), "en_US"}, {tr("German"), "de_DE"},
	};

	VHFPreferences dialog(this);
	dialog.latitude->setText(System::vhf_latitude().str().c_str());
	dialog.longitude->setText(System::vhf_longitude().str().c_str());
	dialog.datetime_utc->setText(System::vhf_time().str().c_str());
	dialog.mmsi->setText(System::vhf_mmsi().str().c_str());
	dialog.mmsi->setReadOnly(System::is_vhf_mmsi_fix());
	dialog.mmsi->setEnabled(!System::is_vhf_mmsi_fix());
	dialog.group->setText(System::vhf_group().str().c_str());
	const QString current = System::lang();
	for (auto i = 0u; i < languages.size(); ++i) {
		dialog.language->addItem(languages[i].first, languages[i].second);
		if (current == languages[i].second)
			dialog.language->setCurrentIndex(i);
	}
	if (dialog.exec() == QDialog::Accepted) {
		engine::Latitude lat;
		if (engine::Latitude::parse(lat, dialog.latitude->text().toStdString()))
			System::vhf_latitude(lat);
		engine::Longitude lon;
		if (engine::Longitude::parse(lon, dialog.longitude->text().toStdString()))
			System::vhf_longitude(lon);
		engine::Date utc;
		if (engine::Date::parse(utc, dialog.datetime_utc->text().toStdString()))
			System::vhf_time(utc);
		System::vhf_mmsi(dialog.mmsi->text().toLong());
		System::vhf_group(dialog.group->text().toLong());
		System::lang(dialog.language->currentData().toString());
		System::save();
	}
}

void MainWindow::on_connection_preferences()
{
	ConnectionPreferences dialog(this);
	dialog.host->setText(System::com_host());
	dialog.port->setText(QString::number(System::com_port()));
	if (dialog.exec() == QDialog::Accepted) {
		System::com_host(dialog.host->text());
		System::com_port(dialog.port->text().toLong());
		System::save();
	}
}

void MainWindow::on_comm_hub() { ether->show(); }

void MainWindow::on_exam_mode(bool checked)
{
	System::exam_mode(checked);
	widget->set_exam_mode(checked);
	set_title();
}

void MainWindow::on_about()
{
	auto app = QCoreApplication::instance();
	QMessageBox::about(this, app->applicationName(),
		tr("%1\nVersion: %2\n\n%3\n\nLicense: %4\n\ngit: %5/%6\n\nAuthor: %7")
			.arg(app->applicationName())
			.arg(app->applicationVersion())
			.arg(tr("Simulation of a VHF radio for maritime purposes."))
			.arg(project_license)
			.arg(git_branch)
			.arg(git_commit_hash)
			.arg("Mario Konrad"));
}

void MainWindow::on_about_qt()
{
	QMessageBox::aboutQt(this, QCoreApplication::instance()->applicationName());
}
}
