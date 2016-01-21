// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__MSGSENDERSOCKET__HPP__
#define __VHFSIM__MSGSENDERSOCKET__HPP__

#include "MsgSender.hpp"
#include "engine/msg.hpp"

class QTcpSocket;

namespace vhfsim
{
class MsgSenderSocket : public MsgSender
{
private:
	QTcpSocket * sock;

public:
	MsgSenderSocket(QTcpSocket *);
	virtual ~MsgSenderSocket();
	virtual int send(const engine::msg_t &) override;
};
}

#endif
