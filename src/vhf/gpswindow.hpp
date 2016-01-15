// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__GPSDIALOG__HPP__
#define __SIMRADRD68__GPSDIALOG__HPP__

#include "ui_gpswindow.h"

class QTimer;

namespace simradrd68
{
class GPSWindow : public QWidget, public Ui::GPSWindow
{
	Q_OBJECT

public:
	GPSWindow(QWidget * parent);

signals:
	void sentence(const std::string & s);

private slots:
	void latitude_edit_finished();
	void longitude_edit_finished();
	void datetime_edit_finished();
	void timeout();
	void toggle_active();

private:
	QTimer * timer;

	void handle_timer(bool is_on);
};
}

#endif
