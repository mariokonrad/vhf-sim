// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "MsgSenderSocket.hpp"
#include <cassert>
#include <QTcpSocket>

namespace vhfsim
{
MsgSenderSocket::MsgSenderSocket(QTcpSocket * s)
	: sock(s)
{
	assert(sock);
}

MsgSenderSocket::~MsgSenderSocket() {}

int MsgSenderSocket::send(const engine::msg_t & m)
{
	using namespace engine;

	assert(sock);

	engine::msg_t data = hton(m);
	auto rc = sock->write(reinterpret_cast<const char *>(&data), sizeof(data));
	return rc == -1 ? -1 : 0;
}
}
