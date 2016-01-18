// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__ETHERWINDOW__HPP__
#define __SIMRADRD68__ETHERWINDOW__HPP__

#include "ui_etherwindow.h"

class QTcpServer;
class QTcpSocket;

namespace simradrd68
{
namespace engine
{
	struct msg_t;
}

class PeerModel;

class EtherWindow : public QWidget, public Ui::EtherWindow
{
	Q_OBJECT

public:
	EtherWindow(QWidget * parent);

private:
	PeerModel * model;
	QTcpServer * server;

	void toggle_server();
	void handle_new_connection();
	void disconnected(QTcpSocket * sock);
	void data_ready(QTcpSocket * sock);
	void server_close();
	void process_vhf(const engine::msg_t & msg);
};
}

#endif
