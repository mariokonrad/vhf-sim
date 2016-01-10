#include "MainWindow.hpp"
#include <QMenuBar>
#include <QCoreApplication>
#include <QMessageBox>
#include "action/About.hpp"
#include "action/AboutQt.hpp"
#include "action/ExitApplication.hpp"

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
}

void MainWindow::create_menubar()
{
	auto menubar = menuBar();

	auto menu_file = menubar->addMenu(tr("&File"));
	menu_file->addAction(action_exit_application);

	auto menu_help = menubar->addMenu(tr("&Help"));
	menu_help->addAction(action_about);
	menu_help->addAction(action_about_qt);
}

void MainWindow::on_about()
{
	QCoreApplication * app = QCoreApplication::instance();
	QMessageBox::about(
		this, app->applicationName(), QString("\n\nVersion: ") + app->applicationVersion());
}

void MainWindow::on_about_qt()
{
	QMessageBox::aboutQt(this, QCoreApplication::instance()->applicationName());
}
}
