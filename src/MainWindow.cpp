#include "MainWindow.hpp"
#include <QMenuBar>
#include <QCoreApplication>
#include <QMessageBox>
#include "action/About.hpp"
#include "action/AboutQt.hpp"
#include "action/ExitApplication.hpp"
#include "action/Fullscreen.hpp"

namespace radiosim
{
MainWindow::MainWindow()
{
	setWindowTitle(QCoreApplication::instance()->applicationName());
	create_actions();
	create_menubar();
}

void MainWindow::create_actions()
{
	action_about = new About(this);
	connect(action_about, SIGNAL(triggered()), this, SLOT(on_about()));

	action_about_qt = new AboutQt(this);
	connect(action_about_qt, SIGNAL(triggered()), this, SLOT(on_about_qt()));

	action_exit_application = new ExitApplication(this);
	connect(action_exit_application, SIGNAL(triggered()), this, SLOT(close()));

	action_toggle_fullscreen = new Fullscreen(this);
	connect(action_toggle_fullscreen, SIGNAL(showFullScreen()), this, SLOT(showFullScreen()));
	connect(action_toggle_fullscreen, SIGNAL(showNormal()), this, SLOT(showNormal()));
}

void MainWindow::create_menubar()
{
	auto menubar = menuBar();

	auto menu_file = menubar->addMenu(tr("&File"));
	menu_file->addAction(action_exit_application);

	auto menu_view = menubar->addMenu(tr("&View"));
	menu_view->addAction(action_toggle_fullscreen);

	auto menu_help = menubar->addMenu(tr("&Help"));
	menu_help->addAction(action_about);
	menu_help->addAction(action_about_qt);
}

void MainWindow::on_about()
{
	QCoreApplication * app = QCoreApplication::instance();
	QMessageBox::about(this, app->applicationName(), tr("Radio-Sim") + tr("\n\nVersion: ")
			+ app->applicationVersion() + tr("\n\nLicense: GPL"));
}

void MainWindow::on_about_qt()
{
	QMessageBox::aboutQt(this, QCoreApplication::instance()->applicationName());
}
}
