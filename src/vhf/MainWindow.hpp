#ifndef __RADIO_SIM__MAINWINDOW__HPP__
#define __RADIO_SIM__MAINWINDOW__HPP__

#include <QMainWindow>

namespace simradrd68
{
class Widget;
class GPSWindow;
class ControlCenter;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

protected:
	void closeEvent(QCloseEvent * event);

private slots:
	void on_about();
	void on_about_qt();
	void on_toggle_fullscreen(bool checked);
	void on_show_buttons(bool checked);
	void on_vhf_preferences();
	void on_connection_preferences();
	void on_connection_open();
	void on_connection_close();
	void on_controlcenter();
	void on_show_gps();
	void on_exam_mode(bool checked);

private:
	Widget * widget;
	GPSWindow * gps;
	ControlCenter * controlcenter;

	void set_title();
};
}

#endif
