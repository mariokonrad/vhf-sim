// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "gpswindow.hpp"
#include "System.hpp"
#include "nmea/nmea_sentence.hpp"
#include <QMessageBox>
#include <QTimer>

namespace vhfsim
{
GPSWindow::GPSWindow(QWidget * parent)
	: QWidget(parent, Qt::Window)
{
	setupUi(this);

	latitude->setText(System::gps_latitude().str().c_str());
	longitude->setText(System::gps_longitude().str().c_str());
	datetime_utc->setText(System::gps_time().str().c_str());

	timer = new QTimer(this);
	timer->setSingleShot(false);
	timer->stop();

	handle_timer(true);

	connect(button_toggle, &QPushButton::clicked, this, &GPSWindow::toggle_active);
	connect(button_close, &QPushButton::clicked, this, &QWidget::hide);
	connect(latitude, &QLineEdit::editingFinished, this, &GPSWindow::latitude_edit_finished);
	connect(longitude, &QLineEdit::editingFinished, this, &GPSWindow::longitude_edit_finished);
	connect(
		datetime_utc, &QLineEdit::editingFinished, this, &GPSWindow::datetime_edit_finished);
	connect(timer, &QTimer::timeout, this, &GPSWindow::timeout);
}

void GPSWindow::latitude_edit_finished()
{
	engine::Latitude data;
	if (engine::Latitude::parse(data, latitude->text().toStdString())) {
		System::gps_latitude(data);
	} else {
		QMessageBox::critical(this, tr("Field Error"),
			tr("The field '%1' has a invalid format.").arg("Latitude"));
		latitude->setText(System::gps_latitude().str().c_str());
		latitude->setFocus();
	}
}

void GPSWindow::longitude_edit_finished()
{
	engine::Longitude data;
	if (engine::Longitude::parse(data, longitude->text().toStdString())) {
		System::gps_longitude(data);
	} else {
		QMessageBox::critical(this, tr("Field Error"),
			tr("The field '%1' has a invalid format.").arg("Longitude"));
		longitude->setText(System::gps_longitude().str().c_str());
		longitude->setFocus();
	}
}

void GPSWindow::datetime_edit_finished()
{
	engine::Date data;
	if (engine::Date::parse(data, datetime_utc->text().toStdString())) {
		System::gps_time(data);
	} else {
		QMessageBox::critical(this, tr("Field Error"),
			tr("The field '%1' has a invalid format.").arg("Date/Time (UTC)"));
		datetime_utc->setText(System::gps_time().str().c_str());
		datetime_utc->setFocus();
	}
}

void GPSWindow::toggle_active() { handle_timer(timer->isActive()); }

void GPSWindow::handle_timer(bool turn_off)
{
	if (turn_off) {
		timer->stop();
		button_toggle->setText(tr("OFF"));
	} else {
		timer->start(1000);
		button_toggle->setText(tr("ON"));
	}
}

void GPSWindow::timeout()
{
	engine::Latitude lat;
	if (!engine::Latitude::parse(lat, latitude->text().toStdString()))
		return;
	engine::Longitude lon;
	if (!engine::Longitude::parse(lon, longitude->text().toStdString()))
		return;
	engine::Date t;
	if (!engine::Date::parse(t, datetime_utc->text().toStdString()))
		return;

	nmea::RMC rmc;
	rmc.set(t);
	rmc.set(lat);
	rmc.set(lon);

	emit sentence(rmc.str());
}
}
