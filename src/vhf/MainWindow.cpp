#include "MainWindow.hpp"
#include <QMenuBar>
#include <QCoreApplication>
#include <QMessageBox>
#include <QDebug>
#include "version.hpp"
#include "Widget.hpp"
#include "System.hpp"

namespace simradrd68
{
MainWindow::MainWindow()
{
	set_title();

	// main widget

	widget = new Widget(this);
	setCentralWidget(widget);

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

	auto action_open_connection = new QAction(tr("Open Connection..."), this);
	action_open_connection->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
	connect(action_open_connection, &QAction::triggered, this, &MainWindow::on_connection_open);

	auto action_close_connection = new QAction(tr("Close Connection"), this);
	connect(
		action_close_connection, &QAction::triggered, this, &MainWindow::on_connection_close);

	auto action_control_center = new QAction(tr("Control Center..."), this);
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

	// menubar

	auto menubar = menuBar();

	auto menu_file = menubar->addMenu(tr("&File"));
	menu_file->addAction(action_open_connection);
	menu_file->addAction(action_close_connection);
	menu_file->addAction(action_control_center);
	menu_file->addAction(action_show_gps);
	menu_file->addSeparator();
	menu_file->addAction(action_vhf_preferences);
	menu_file->addAction(action_connection_preferences);
	menu_file->addSeparator();
	menu_file->addAction(action_exam_mode);
	menu_file->addSeparator();
	menu_file->addAction(action_exit_application);

	auto menu_view = menubar->addMenu(tr("&View"));
	menu_view->addAction(action_toggle_fullscreen);
	menu_view->addAction(action_show_buttons);

	auto menu_help = menubar->addMenu(tr("&Help"));
	menu_help->addAction(action_about);
	menu_help->addAction(action_about_qt);
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

void MainWindow::on_connection_open() { qDebug() << __PRETTY_FUNCTION__ << "NOT IMPLEMENTED"; }

void MainWindow::on_connection_close() { qDebug() << __PRETTY_FUNCTION__ << "NOT IMPLEMENTED"; }

void MainWindow::on_controlcenter() { qDebug() << __PRETTY_FUNCTION__ << "NOT IMPLEMENTED"; }

void MainWindow::on_show_gps() { qDebug() << __PRETTY_FUNCTION__ << "NOT IMPLEMENTED"; }

void MainWindow::on_vhf_preferences() { qDebug() << __PRETTY_FUNCTION__ << "NOT IMPLEMENTED"; }

void MainWindow::on_connection_preferences() { qDebug() << __PRETTY_FUNCTION__ << "NOT IMPLEMENTED"; }

void MainWindow::on_exam_mode(bool checked)
{
	System::exam_mode(checked);
	widget->set_exam_mode(checked);
	set_title();
}

void MainWindow::on_about()
{
	auto app = QCoreApplication::instance();
	QMessageBox::about(this, app->applicationName(), tr("%1\n\nVersion: %2\n\nLicense: GPL")
														 .arg(app->applicationName())
														 .arg(app->applicationVersion()));
}

void MainWindow::on_about_qt()
{
	QMessageBox::aboutQt(this, QCoreApplication::instance()->applicationName());
}
}
