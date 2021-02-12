// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_VHFPREFERENCES_HPP
#define VHFSIM_VHFPREFERENCES_HPP

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
