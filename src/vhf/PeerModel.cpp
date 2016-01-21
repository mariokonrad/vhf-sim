// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "PeerModel.hpp"

namespace vhfsim
{
PeerModel::PeerModel(QObject * parent)
	: QAbstractItemModel(parent)
{
}

PeerModel::~PeerModel() {}

int PeerModel::columnCount(const QModelIndex & parent) const
{
	return (parent.isValid()) ? 0 : 2;
}

QVariant PeerModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (role != Qt::DisplayRole)
		return QVariant();
	if (index.row() < 0)
		return QVariant();
	if (index.row() >= static_cast<int>(peers.size()))
		return QVariant();
	switch (index.column()) {
		case 0:
			return peers.at(index.row()).mmsi;
		case 1:
			return peers.at(index.row()).group;
	}
	return QVariant();
}

QModelIndex PeerModel::index(int row, int column, const QModelIndex &) const
{
	return createIndex(row, column);
}

QModelIndex PeerModel::parent(const QModelIndex &) const { return QModelIndex(); }

int PeerModel::rowCount(const QModelIndex &) const { return peers.size(); }

QVariant PeerModel::headerData(int, Qt::Orientation, int) const { return QVariant(); }

void PeerModel::insert(const Entry & entry)
{
	// the list of peers will never be large, the easiest way is to reset the model

	beginResetModel();
	peers.push_back(entry);
	endResetModel();
}

void PeerModel::erase(QTcpSocket * sock)
{
	auto i = std::find_if(peers.begin(), peers.end(),
		[sock](const Entry & entry) { return entry.socket == sock; });
	if (i != peers.end()) {
		beginResetModel();
		peers.erase(i);
		endResetModel();
	}
}
}
