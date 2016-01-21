// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__ENGINE__CONTROLLER__HPP__
#define __VHFSIM__ENGINE__CONTROLLER__HPP__

#include "msg.hpp"
#include <string>

namespace vhfsim
{
namespace engine
{
class Controller
{
public:
	virtual ~Controller() {}

	// buttons and key bindings
	virtual void bind_button_circle(int, int, int, int, int, int) = 0;
	virtual void bind_button_rect(int, int, int, int, int, int, int) = 0;
	virtual void bind_key(int, int, int) = 0;

	// timer
	virtual void timer_create(int) = 0;
	virtual void timer_delete(int) = 0;
	virtual void timer_start(int, int, bool) = 0;
	virtual void timer_stop(int) = 0;

	// message
	virtual int msg_recv(msg_t &) = 0;
	virtual int msg_send(const msg_t &) = 0;
	virtual void process(const msg_t &) = 0;
	virtual void bind_msg(int) = 0;

	// gps
	virtual void gps_process(const std::string &) = 0;
	virtual void bind_gps(int) = 0;
	virtual void set_exam_mode(bool) = 0;
};
}
}

#endif
