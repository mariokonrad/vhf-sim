// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "vhfpreferences.hpp"
#include "System.hpp"
#include <QDebug>

namespace simradrd68
{
VHFPreferences::VHFPreferences(QWidget * parent)
	: QDialog(parent)
{
	setupUi(this);
	connect(this, &QDialog::finished, this, &VHFPreferences::check);
}

void VHFPreferences::check(int result)
{
	if (result != 1)
		return;

	qDebug() << __PRETTY_FUNCTION__ << "TODO: save data";
}
}
