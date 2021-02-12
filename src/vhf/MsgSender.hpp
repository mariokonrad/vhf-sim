// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_MSGSENDER_HPP
#define VHFSIM_MSGSENDER_HPP

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
