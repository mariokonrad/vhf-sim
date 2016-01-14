// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__MSGSENDER__HPP__
#define __SIMRADRD68__MSGSENDER__HPP__

#include "engine/msg.hpp"

namespace simradrd68
{
class MsgSender
{
public:
	virtual ~MsgSender() {}
	virtual int send(const engine::msg_t &) = 0;
};
}

#endif
