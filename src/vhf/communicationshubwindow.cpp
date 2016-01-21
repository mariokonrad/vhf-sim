// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "communicationshubwindow.hpp"
#include "PeerModel.hpp"
#include "engine/msg.hpp"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

namespace vhfsim
{
CommunicationsHubWindow::CommunicationsHubWindow(QWidget * parent)
	: QWidget(parent, Qt::Window)
	, server(nullptr)
{
	setupUi(this);

	port->setText("9540");

	connect(button_close, &QPushButton::clicked, this, &CommunicationsHubWindow::close);
	connect(
		button_toggle, &QPushButton::clicked, this, [this](bool) { this->toggle_server(); });

	model = new PeerModel(this);
	peer_view->setModel(model);

	server_close();
}

void CommunicationsHubWindow::server_close()
{
	button_toggle->setText(tr("OFF"));
	if (server) {
		delete server;
		server = nullptr;
	}
}

void CommunicationsHubWindow::toggle_server()
{
	if (server) {
		server_close();
	} else {
		server = new QTcpServer(this);
		if (server->listen(QHostAddress::Any, port->text().toLong())) {
			button_toggle->setText(tr("ON"));
			connect(server, &QTcpServer::newConnection, this,
				[this]() { this->handle_new_connection(); });
		} else {
			QMessageBox::critical(this, tr("Connection Error"), tr("Unable to open server"));
			server_close();
		}
	}
}

void CommunicationsHubWindow::handle_new_connection()
{
	while (server->hasPendingConnections()) {
		auto connection = server->nextPendingConnection();
		connect(connection, &QTcpSocket::disconnected, this,
			[this, connection]() { this->disconnected(connection); });
		connect(connection, &QTcpSocket::readyRead, this,
			[this, connection]() { this->data_ready(connection); });
	}
}

void CommunicationsHubWindow::disconnected(QTcpSocket * sock)
{
	disconnect(sock);
	model->erase(sock);
}

void CommunicationsHubWindow::process_vhf(const engine::msg_t & msg, const engine::msg_t & org)
{
	for (auto const & peer : *model) {
		if (peer.mmsi == msg.mmsi)
			continue;
		peer.socket->write(reinterpret_cast<const char *>(&org), sizeof(org));
	}
}

void CommunicationsHubWindow::data_ready(QTcpSocket * sock)
{
	using namespace engine;

	msg_t msg;
	auto rc = sock->read(reinterpret_cast<char *>(&msg), sizeof(msg));
	if (rc != sizeof(msg)) {
		delete sock;
		return;
	}

	auto m = ntoh(msg);
	switch (m.type) {
		case MSG_PULSE:
			model->insert({sock, m.mmsi, m.group});
			break;

		case MSG_VHF:
			process_vhf(m, msg);
			break;
	}
}
}
