// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__PEERMODEL__HPP__
#define __SIMRADRD68__PEERMODEL__HPP__

#include <vector>
#include <QAbstractItemModel>
#include <QModelIndex>

class QTcpSocket;

namespace simradrd68
{
class PeerModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	struct Entry {
		QTcpSocket * socket;
		QString mmsi;
		QString group;
	};

public:
	PeerModel(QObject *);
	virtual ~PeerModel();

	virtual int columnCount(const QModelIndex & = QModelIndex()) const override;
	virtual QVariant data(
		const QModelIndex & = QModelIndex(), int = Qt::DisplayRole) const override;
	virtual QModelIndex index(int, int, const QModelIndex & = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex &) const override;
	virtual int rowCount(const QModelIndex & = QModelIndex()) const override;
	virtual QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const override;

	void insert(const Entry & entry);
	void erase(QTcpSocket * sock);

private:
	using Peers = std::vector<Entry>;

	Peers peers;

public:
	using const_iterator = Peers::const_iterator;

	const_iterator begin() const { return peers.begin(); }
	const_iterator end() const { return peers.end(); }
};
}

#endif
