#ifndef __RADIO_SIM__MAINWINDOW__HPP__
#define __RADIO_SIM__MAINWINDOW__HPP__

#include <QMainWindow>

class QAction;

namespace radiosim
{
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void on_about();
	void on_about_qt();

private:
	void create_actions();
	void create_menubar();

	QAction * action_about;
	QAction * action_about_qt;
	QAction * action_exit_application;
};
}

#endif
