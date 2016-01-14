// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__CONNECTIONPREFERENCES__HPP__
#define __SIMRADRD68__CONNECTIONPREFERENCES__HPP__

#include "ui_connectionpreferences.h"

namespace simradrd68
{
class ConnectionPreferences : public QDialog, public Ui::ConnectionPreferences
{
	Q_OBJECT

public:
	ConnectionPreferences(QWidget * parent);
};
}

#endif
