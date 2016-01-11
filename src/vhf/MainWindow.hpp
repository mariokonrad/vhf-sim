#ifndef __RADIO_SIM__MAINWINDOW__HPP__
#define __RADIO_SIM__MAINWINDOW__HPP__

#include <QMainWindow>

class QAction;

namespace simradrd68
{
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void on_about();
	void on_about_qt();
	void on_toggle_fullscreen(bool checked);
};
}

#endif
