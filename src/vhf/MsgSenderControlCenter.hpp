// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__MSGSENDERCONTROLCENTER__HPP__
#define __VHFSIM__MSGSENDERCONTROLCENTER__HPP__

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
