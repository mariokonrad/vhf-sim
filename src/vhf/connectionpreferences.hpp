// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_CONNECTIONPREFERENCES_HPP
#define VHFSIM_CONNECTIONPREFERENCES_HPP

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
