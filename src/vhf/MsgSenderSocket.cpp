// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "MsgSenderSocket.hpp"
#include <cassert>
#include <QTcpSocket>
#include <QDebug>

namespace simradrd68
{
MsgSenderSocket::MsgSenderSocket(QTcpSocket * s)
	: sock(s)
{
	assert(sock);
}

MsgSenderSocket::~MsgSenderSocket() {}

int MsgSenderSocket::send(const engine::msg_t & m)
{
	assert(sock);

	engine::msg_t data = hton(m);
	auto rc = sock->write(reinterpret_cast<const char *>(&data), sizeof(data));
	qDebug() << __PRETTY_FUNCTION__ << "rc=" << rc << "sizeof=" << sizeof(data);
	return rc == -1 ? -1 : 0;
}
}
