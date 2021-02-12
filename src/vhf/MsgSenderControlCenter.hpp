// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_MSGSENDERCONTROLCENTER_HPP
#define VHFSIM_MSGSENDERCONTROLCENTER_HPP

#include "MsgSender.hpp"

namespace vhfsim
{
class ControlCenter;

class MsgSenderControlCenter : public MsgSender
{
public:
	MsgSenderControlCenter(ControlCenter * cc);

	virtual int send(const engine::msg_t & m) override;

private:
	ControlCenter * control_center;
};
}

#endif
