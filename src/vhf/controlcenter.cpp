// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "controlcenter.hpp"
#include "System.hpp"
#include <vector>
#include <QMessageBox>

namespace simradrd68
{
static const std::vector<int> work_channels
	= {6, 8, 9, 10, 12, 13, 15, 16, 17, 67, 69, 72, 73, 77};

// TODO: duplex channels configurable
static const std::vector<int> duplex_channels = {1, 2, 3, 4, 5, 23, 24, 25, 26, 27, 82, 83};

static const std::vector<std::pair<QString, int>> designations = {
	{QObject::tr("Undefined"), engine::DSC_DES_UNDEFINED},
	{QObject::tr("Abandoning"), engine::DSC_DES_ABANDONING},
	{QObject::tr("Piracy"), engine::DSC_DES_PIRACY},
	{QObject::tr("M.O.B."), engine::DSC_DES_MOB}, {QObject::tr("Fire"), engine::DSC_DES_FIRE},
	{QObject::tr("Flooding"), engine::DSC_DES_FLOODING},
	{QObject::tr("Collision"), engine::DSC_DES_COLLISION},
	{QObject::tr("Grounding"), engine::DSC_DES_GROUNDING},
	{QObject::tr("Listing"), engine::DSC_DES_LISTING},
	{QObject::tr("Sinking"), engine::DSC_DES_SINKING},
	{QObject::tr("Adrift"), engine::DSC_DES_ADRIFT}};

ControlCenter::ControlCenter(QWidget * parent)
	: QWidget(parent, Qt::Window)
{
	setupUi(this);

	latitude->setText(System::cc_latitude().str().c_str());
	longitude->setText(System::cc_longitude().str().c_str());
	datetime_utc->setText(System::cc_time().str().c_str());
	mmsi->setText(System::cc_mmsi().str().c_str());
	group->setText(System::cc_group().str().c_str());
	for (auto const & channel : work_channels)
		working_channel->addItem(QString::number(channel));
	for (auto const & d : designations)
		designation->addItem(d.first, d.second);

	connect(button_close, &QPushButton::clicked, this, &QWidget::hide);

	connect(button_routine, &QPushButton::clicked, this, &ControlCenter::send_routine);
	connect(button_group, &QPushButton::clicked, this, &ControlCenter::send_group);
	connect(button_safety, &QPushButton::clicked, this, &ControlCenter::send_safety);
	connect(button_urgency, &QPushButton::clicked, this, &ControlCenter::send_urgency);
	connect(button_distress, &QPushButton::clicked, this, &ControlCenter::send_distress);
	connect(button_confirm, &QPushButton::clicked, this, &ControlCenter::send_confirm);
	connect(button_cancel, &QPushButton::clicked, this, &ControlCenter::on_cancel);

	connect(
		latitude, &QLineEdit::editingFinished, this, &ControlCenter::latitude_edit_finished);
	connect(
		longitude, &QLineEdit::editingFinished, this, &ControlCenter::longitude_edit_finished);
	connect(datetime_utc, &QLineEdit::editingFinished, this,
		&ControlCenter::datetime_edit_finished);
	connect(mmsi, &QLineEdit::editingFinished, this, &ControlCenter::mmsi_edit_finished);
	connect(group, &QLineEdit::editingFinished, this, &ControlCenter::group_edit_finished);
	connect(
		receive_all, &QCheckBox::stateChanged, this, &ControlCenter::receive_all_state_change);

	set_send_new(true);
	set_ack(false);
	set_cancel(false);

	reset();
}

engine::msg_t ControlCenter::prepare_msg()
{
	using namespace engine;

	msg_t m;
	msg_set_latitude(m, System::cc_latitude().str());
	msg_set_longitude(m, System::cc_longitude().str());
	m.valid_time = 1;
	System::cc_time().str(m.time);
	System::cc_mmsi().str(m.mmsi);
	memset(&m.group, 0, sizeof(m.group));
	m.channel = 70;
	m.work_channel = working_channel->currentText().toLong();
	m.power = 25;

	memset(&m.dsc.mmsi, 0, sizeof(m.dsc.mmsi));

	return m;
}

void ControlCenter::do_send(const engine::msg_t & m) { emit send(m); }

void ControlCenter::send_routine()
{
	using namespace engine;

	msg_t m = prepare_msg();

	m.dsc.type = DSC_ROUTINE;
	m.dsc.recv = DSC_INDIVIDUAL;
	m.dsc.designation = DSC_DES_UNDEFINED;
	System::vhf_mmsi().str(m.dsc.mmsi);

	set_send_new(false);
	set_ack(false);
	set_cancel(true);

	do_send(m);
}

void ControlCenter::send_group()
{
	using namespace engine;

	msg_t m = prepare_msg();

	System::cc_group().str(m.group);

	m.dsc.type = DSC_ROUTINE;
	m.dsc.recv = DSC_GROUP;
	m.dsc.designation = DSC_DES_UNDEFINED;
	System::vhf_mmsi().str(m.dsc.mmsi);

	set_send_new(true);
	set_ack(false);
	set_cancel(true);

	do_send(m);
}

void ControlCenter::send_safety()
{
	using namespace engine;

	msg_t m = prepare_msg();

	m.dsc.type = DSC_SAFETY;
	m.dsc.recv = DSC_ALL;
	m.dsc.designation = DSC_DES_UNDEFINED;

	set_send_new(true);
	set_ack(false);
	set_cancel(true);

	do_send(m);
}

void ControlCenter::send_urgency()
{
	using namespace engine;

	msg_t m = prepare_msg();

	m.work_channel = 16;

	m.dsc.type = DSC_URGENCY;
	m.dsc.recv = DSC_ALL;
	m.dsc.designation = DSC_DES_UNDEFINED;

	set_send_new(true);
	set_ack(false);
	set_cancel(true);

	do_send(m);
}

void ControlCenter::send_distress()
{
	using namespace engine;

	msg_t m = prepare_msg();

	m.work_channel = 16;

	m.dsc.type = DSC_ALERT;
	m.dsc.recv = DSC_ALL;
	m.dsc.designation = designation->currentData().toString().toLong();

	set_send_new(true);
	set_ack(false);
	set_cancel(true);

	do_send(m);
}

void ControlCenter::send_confirm()
{
	if (!received_msg)
		return;

	using namespace engine;

	msg_t m = prepare_msg();

	m.dsc.type = DSC_ACK;
	m.dsc.recv = received_msg->dsc.recv;
	m.dsc.designation = DSC_DES_UNDEFINED;
	System::vhf_mmsi().str(m.dsc.mmsi);

	if (MMSI(received_msg->dsc.mmsi).is_coast_station()) {
		std::default_random_engine random_engine(device());
		std::uniform_int_distribution<int> uniform_dist(0, duplex_channels.size());
		int i = uniform_dist(random_engine);
		m.work_channel = duplex_channels[i];
	} else {
		m.work_channel = received_msg->work_channel;
	}

	set_send_new(true);
	set_ack(false);
	set_cancel(true);

	do_send(m);

	reset();
	set_send_new(true);
	set_ack(false);
	set_cancel(false);
	received_msg.reset();
}

void ControlCenter::on_cancel()
{
	reset();
	set_send_new(true);
	set_ack(false);
	set_cancel(false);
}

void ControlCenter::receive_all_state_change(bool state) { System::cc_all_msg(state); }

void ControlCenter::receive(const engine::msg_t & m)
{
	using namespace engine;

	// process VHF messages, ignore all others
	if (m.type != MSG_VHF)
		return;

	// check for being receiver (message to all, to group or to mmsi)
	if (!(System::cc_all_msg() || (m.dsc.recv == DSC_ALL)
			|| ((m.dsc.recv == DSC_GROUP) && (System::cc_group() == m.dsc.mmsi))
			|| ((m.dsc.recv == DSC_INDIVIDUAL) && (System::cc_mmsi() == m.dsc.mmsi))))
		return;

	// copy message
	received_msg.reset(new engine::msg_t);
	memcpy(received_msg.get(), &m, sizeof(engine::msg_t));

	// check for msg, CANCEL??
	switch (static_cast<DSC_TYPE>(received_msg->dsc.type)) {
		case DSC_ROUTINE:
			set(*received_msg);
			if (received_msg->dsc.recv != DSC_GROUP) {
				set_send_new(false);
				set_ack(true);
			} else {
				set_send_new(true);
				set_ack(false);
			}
			set_cancel(true);
			break;

		case DSC_SAFETY:
		case DSC_URGENCY:
			set(*received_msg);
			set_send_new(false);
			set_ack(false);
			set_cancel(true);
			break;

		case DSC_ALERT:
			set(*received_msg);
			set_send_new(false);
			set_ack(true);
			set_cancel(true);
			break;

		case DSC_ACK:
			set(*received_msg);
			set_send_new(true);
			set_ack(false);
			set_cancel(false);
			break;

		default:
		case DSC_INVALID_TYPE:
			// ignore!
			break;
	}
}

void ControlCenter::set(const engine::msg_t & m)
{
	using namespace engine;

	switch (m.dsc.type) {
		case DSC_ROUTINE: {
			QString t;
			if (m.dsc.recv == DSC_INDIVIDUAL) {
				t += tr("Routine Call") + "\n";
			} else if (m.dsc.recv == DSC_GROUP) {
				t += tr("Group Call") + "\n";
			} else {
				t += tr("**INVALID**") + "\n";
			}
			t += tr("From ") + QString(m.mmsi) + "\n";
			t += tr("On %1\n").arg(m.work_channel);
			received_message->setText(t);
		} break;

		case DSC_SAFETY: {
			QString t;
			t += tr("Safety Call") + "\n";
			t += tr("From ") + QString(m.mmsi) + "\n";
			t += tr("On %1\n").arg(m.work_channel);
			received_message->setText(t);
		} break;

		case DSC_URGENCY: {
			QString t;
			t += tr("Urgency Call") + "\n";
			t += tr("From ") + QString(m.mmsi) + "\n";
			t += tr("On %1\n").arg(m.work_channel);
			received_message->setText(t);
		} break;

		case DSC_ALERT: {
			QString t;
			Date d;
			Date::parse(d, m.time);

			t += tr("Distress Alert");
			if (m.valid_time) {
				t += tr("  %1:%2")
						 .arg(d.hour(), 2, QLatin1Char('0'))
						 .arg(d.min(), 2, QLatin1Char('0'));
			}
			t += "\n";
			t += tr("From ") + QString(m.mmsi) + "\n";
			if (m.valid_lat && m.valid_lon) {
				t += QString(m.lat) + "  " + QString(m.lon) + "\n";
			} else {
				t += tr("**No Position**") + "\n";
			}
			auto i = std::find_if(designations.begin(), designations.end(),
				[m](const std::pair<QString, int> & d) {
					return m.dsc.designation == d.second;
				});
			if (i != designations.end()) {
				t += i->first;
				t += "\n";
			}
			received_message->setText(t);
		} break;

		case DSC_ACK: // ignore
			reset();
			break;
		case DSC_INVALID_TYPE: // ignore
			reset();
			break;
	}
}

void ControlCenter::reset() { received_message->setText("-"); }

void ControlCenter::latitude_edit_finished()
{
	engine::Latitude data;
	if (engine::Latitude::parse(data, latitude->text().toStdString())) {
		System::cc_latitude(data);
	} else {
		QMessageBox::critical(this, tr("Field Error"),
			tr("The field '%1' has a invalid format.").arg("Latitude"));
		latitude->setText(System::cc_latitude().str().c_str());
		latitude->setFocus();
	}
}

void ControlCenter::longitude_edit_finished()
{
	engine::Longitude data;
	if (engine::Longitude::parse(data, longitude->text().toStdString())) {
		System::cc_longitude(data);
	} else {
		QMessageBox::critical(this, tr("Field Error"),
			tr("The field '%1' has a invalid format.").arg("Longitude"));
		longitude->setText(System::cc_longitude().str().c_str());
		longitude->setFocus();
	}
}

void ControlCenter::datetime_edit_finished()
{
	engine::Date data;
	if (engine::Date::parse(data, datetime_utc->text().toStdString())) {
		System::cc_time(data);
	} else {
		QMessageBox::critical(this, tr("Field Error"),
			tr("The field '%1' has a invalid format.").arg("Date/Time (UTC)"));
		datetime_utc->setText(System::cc_time().str().c_str());
		datetime_utc->setFocus();
	}
}

void ControlCenter::mmsi_edit_finished() { System::cc_mmsi(mmsi->text().toStdString()); }

void ControlCenter::group_edit_finished() { System::cc_group(group->text().toStdString()); }

void ControlCenter::set_send_new(bool flag)
{
	button_routine->setEnabled(flag);
	button_group->setEnabled(flag);
	button_safety->setEnabled(flag);
	button_urgency->setEnabled(flag);
	button_distress->setEnabled(flag);
}

void ControlCenter::set_ack(bool flag) { button_confirm->setEnabled(flag); }

void ControlCenter::set_cancel(bool flag) { button_cancel->setEnabled(flag); }
}
