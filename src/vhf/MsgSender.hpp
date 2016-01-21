// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__MSGSENDER__HPP__
#define __VHFSIM__MSGSENDER__HPP__

#include "engine/msg.hpp"

namespace vhfsim
{
class MsgSender
{
public:
	virtual ~MsgSender() {}
	virtual int send(const engine::msg_t &) = 0;
};
}

#endif
