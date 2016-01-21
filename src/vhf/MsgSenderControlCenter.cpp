// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "MsgSenderControlCenter.hpp"
#include "controlcenter.hpp"

namespace vhfsim
{
MsgSenderControlCenter::MsgSenderControlCenter(ControlCenter * cc)
	: control_center(cc)
{
}

int MsgSenderControlCenter::send(const engine::msg_t & m)
{
	control_center->receive(m);
	return 0;
}
}
