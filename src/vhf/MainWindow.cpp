#include "MainWindow.hpp"
#include <QMenuBar>
#include <QCoreApplication>
#include <QMessageBox>
#include "version.hpp"

namespace simradrd68
{
MainWindow::MainWindow()
{
	setWindowTitle(QCoreApplication::instance()->applicationName());

	// actions

	auto action_about = new QAction(tr("About"), this);
	action_about->setStatusTip(project_name);
	connect(action_about, SIGNAL(triggered()), this, SLOT(on_about()));

	auto action_about_qt = new QAction(tr("About Qt"), this);
	action_about_qt->setStatusTip(tr("Shows information about Qt"));
	connect(action_about_qt, SIGNAL(triggered()), this, SLOT(on_about_qt()));

	auto action_exit_application = new QAction(tr("E&xit"), this);
	action_exit_application->setStatusTip(tr("Closes the application"));
	connect(action_exit_application, SIGNAL(triggered()), this, SLOT(close()));

	auto action_toggle_fullscreen = new QAction(tr("Fullscreen"), this);
	action_toggle_fullscreen->setStatusTip(tr("Shows the window in fullscreen"));
	action_toggle_fullscreen->setCheckable(true);
	action_toggle_fullscreen->setChecked(false);
	connect(action_toggle_fullscreen, SIGNAL(toggled(bool)), this,
		SLOT(on_toggle_fullscreen(bool)));

	// menubar

	auto menubar = menuBar();

	auto menu_file = menubar->addMenu(tr("&File"));
	menu_file->addAction(action_exit_application);

	auto menu_view = menubar->addMenu(tr("&View"));
	menu_view->addAction(action_toggle_fullscreen);

	auto menu_help = menubar->addMenu(tr("&Help"));
	menu_help->addAction(action_about);
	menu_help->addAction(action_about_qt);
}

void MainWindow::on_toggle_fullscreen(bool checked)
{
	if (checked) {
		showFullScreen();
	} else {
		showNormal();
	}
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
