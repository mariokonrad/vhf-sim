// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "etherwindow.hpp"
#include "PeerModel.hpp"
#include "engine/msg.hpp"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

namespace simradrd68
{
EtherWindow::EtherWindow(QWidget * parent)
	: QWidget(parent, Qt::Window)
	, server(nullptr)
{
	setupUi(this);

	port->setText("9540");

	connect(button_close, &QPushButton::clicked, this, &EtherWindow::close);
	connect(button_toggle, &QPushButton::clicked, [this](bool) { this->toggle_server(); });

	model = new PeerModel(this);
	peer_view->setModel(model);

	server_close();
}

void EtherWindow::server_close()
{
	button_toggle->setText(tr("OFF"));
	if (server) {
		delete server;
		server = nullptr;
	}
}

void EtherWindow::toggle_server()
{
	if (server) {
		server_close();
	} else {
		server = new QTcpServer(this);
		if (server->listen(QHostAddress::Any, port->text().toLong())) {
			button_toggle->setText(tr("ON"));
			connect(server, &QTcpServer::newConnection,
				[this]() { this->handle_new_connection(); });
		} else {
			QMessageBox::critical(this, tr("Connection Error"), tr("Unable to open server"));
			server_close();
		}
	}
}

void EtherWindow::handle_new_connection()
{
	while (server->hasPendingConnections()) {
		auto connection = server->nextPendingConnection();
		connect(connection, &QTcpSocket::disconnected,
			[this, connection]() { this->disconnected(connection); });
		connect(connection, &QTcpSocket::readyRead,
			[this, connection]() { this->data_ready(connection); });
	}
}

void EtherWindow::disconnected(QTcpSocket * sock)
{
	disconnect(sock);
	model->erase(sock);
}

void EtherWindow::process_vhf(const engine::msg_t & msg)
{
	for (auto const & peer : *model) {
		if (peer.mmsi == msg.mmsi)
			continue;
		auto rc = peer.socket->write(reinterpret_cast<const char *>(&msg), sizeof(msg));
		if (rc != sizeof(msg)) {
			qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "error, rc=" << rc;
		}
	}
}

void EtherWindow::data_ready(QTcpSocket * sock)
{
	using namespace engine;

	msg_t msg;
	msg_init(msg);
	auto rc = sock->read(reinterpret_cast<char *>(&msg), sizeof(msg));
	if (rc != sizeof(msg)) {
		delete sock;
		return;
	}

	msg = ntoh(msg);
	switch (msg.type) {
		case MSG_PULSE:
			model->insert({sock, msg.mmsi, msg.group});
			break;

		case MSG_VHF:
			process_vhf(msg);
			break;
	}
}
}
