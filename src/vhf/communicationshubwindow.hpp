// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__COMMUNICATIONSHUBWINDOW__HPP__
#define __VHFSIM__COMMUNICATIONSHUBWINDOW__HPP__

#include "ui_communicationshubwindow.h"

class QTcpServer;
class QTcpSocket;

namespace vhfsim
{
namespace engine
{
struct msg_t;
}

class PeerModel;

class CommunicationsHubWindow : public QWidget, public Ui::CommunicationsHubWindow
{
	Q_OBJECT

public:
	CommunicationsHubWindow(QWidget * parent);

private:
	PeerModel * model;
	QTcpServer * server;

	void toggle_server();
	void handle_new_connection();
	void disconnected(QTcpSocket * sock);
	void data_ready(QTcpSocket * sock);
	void server_close();
	void process_vhf(const engine::msg_t & msg, const engine::msg_t & org);
};
}

#endif
