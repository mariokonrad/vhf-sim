// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__CONNECTIONPREFERENCES__HPP__
#define __VHFSIM__CONNECTIONPREFERENCES__HPP__

#include "ui_connectionpreferences.h"

namespace vhfsim
{
class ConnectionPreferences : public QDialog, public Ui::ConnectionPreferences
{
	Q_OBJECT

public:
	ConnectionPreferences(QWidget * parent);
};
}

#endif
