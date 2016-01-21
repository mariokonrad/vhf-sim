// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__VHFPREFERENCES__HPP__
#define __VHFSIM__VHFPREFERENCES__HPP__

#include "ui_vhfpreferences.h"

namespace vhfsim
{
class VHFPreferences : public QDialog, public Ui::VHFPreferences
{
	Q_OBJECT

public:
	VHFPreferences(QWidget * parent);
};
}

#endif
