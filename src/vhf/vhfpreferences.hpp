// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__VHFPREFERENCES__HPP__
#define __SIMRADRD68__VHFPREFERENCES__HPP__

#include "ui_vhfpreferences.h"

namespace simradrd68
{
class VHFPreferences : public QDialog, Ui::VHFPreferences
{
	Q_OBJECT

public:
	VHFPreferences(QWidget * parent);

private slots:
	void check(int result);
};
}

#endif
