// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__CONTROLCENTER__HPP__
#define __SIMRADRD68__CONTROLCENTER__HPP__

#include "ui_controlcenter.h"
#include "engine/msg.hpp"
#include <memory>
#include <random>

namespace engine
{
struct msg_t;
}

namespace simradrd68
{
class ControlCenter : public QWidget, public Ui::ControlCenter
{
	Q_OBJECT

public:
	ControlCenter(QWidget * parent);

signals:
	void send(const engine::msg_t & m);

public slots:
	void receive(const engine::msg_t & m);

private slots:
	void send_routine();
	void send_group();
	void send_safety();
	void send_urgency();
	void send_distress();
	void send_confirm();
	void on_cancel();

	void latitude_edit_finished();
	void longitude_edit_finished();
	void datetime_edit_finished();
	void mmsi_edit_finished();
	void group_edit_finished();
	void receive_all_state_change(bool state);

private:
	using msg_ptr = std::unique_ptr<engine::msg_t>;

	msg_ptr received_msg;

    std::random_device device;

	engine::msg_t prepare_msg();
	void set(const engine::msg_t & m);
	void do_send(const engine::msg_t & m);
	void reset();
	void set_send_new(bool);
	void set_ack(bool);
	void set_cancel(bool);
};
}

#endif
