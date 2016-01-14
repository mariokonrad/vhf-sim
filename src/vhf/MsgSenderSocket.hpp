// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__MSGSENDERSOCKET__HPP__
#define __SIMRADRD68__MSGSENDERSOCKET__HPP__

#include "MsgSender.hpp"
#include "engine/msg.hpp"

class QTcpSocket;

namespace simradrd68
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
