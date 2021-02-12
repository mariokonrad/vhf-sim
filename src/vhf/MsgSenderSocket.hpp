// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_MSGSENDERSOCKET_HPP
#define VHFSIM_MSGSENDERSOCKET_HPP

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
