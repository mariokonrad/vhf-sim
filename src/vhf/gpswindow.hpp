// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__GPSDIALOG__HPP__
#define __SIMRADRD68__GPSDIALOG__HPP__

#include "ui_gpswindow.h"

namespace simradrd68
{
class GPSWindow : public QWidget, public Ui::GPSWindow
{
	Q_OBJECT

public:
	GPSWindow(QWidget * parent);
};
}

#endif
