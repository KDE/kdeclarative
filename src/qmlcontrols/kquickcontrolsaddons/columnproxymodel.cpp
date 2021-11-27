/*
    SPDX-FileCopyrightText: 2012 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "columnproxymodel.h"

ColumnProxyModel::ColumnProxyModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_column(0)
    , m_sourceModel(nullptr)
{
}

void ColumnProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    if (sourceModel == m_sourceModel) {
        return;
    }

    beginResetModel();
    if (m_sourceModel) {
        disconnect(m_sourceModel, &QObject::destroyed, this, &ColumnProxyModel::sourceDestroyed);

        disconnect(m_sourceModel, &QAbstractItemModel::dataChanged, this, &ColumnProxyModel::considerDataChanged);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &ColumnProxyModel::considerRowsAboutToBeInserted);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, &ColumnProxyModel::considerRowsAboutToBeMoved);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &ColumnProxyModel::considerRowsAboutToBeRemoved);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsInserted, this, &ColumnProxyModel::considerRowsInserted);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsMoved, this, &ColumnProxyModel::considerRowsMoved);
        disconnect(m_sourceModel, &QAbstractItemModel::rowsRemoved, this, &ColumnProxyModel::considerRowsRemoved);

        // TODO: QAbstractItemModel::modelAboutToBeReset and QAbstractItemModel::modelReset are private signals
        // we can connect to them, but can't emit them
        disconnect(m_sourceModel, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
        disconnect(m_sourceModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));

        disconnect(m_sourceModel, &QAbstractItemModel::headerDataChanged, this, &QAbstractItemModel::headerDataChanged);
        disconnect(m_sourceModel, &QAbstractItemModel::layoutAboutToBeChanged, this, &QAbstractItemModel::layoutAboutToBeChanged);
        disconnect(m_sourceModel, &QAbstractItemModel::layoutChanged, this, &QAbstractItemModel::layoutChanged);
    }
    m_sourceModel = sourceModel;
    if (m_sourceModel) {
        connect(m_sourceModel, &QObject::destroyed, this, &ColumnProxyModel::sourceDestroyed);

        connect(m_sourceModel, &QAbstractItemModel::dataChanged, this, &ColumnProxyModel::considerDataChanged);
        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &ColumnProxyModel::considerRowsAboutToBeInserted);
        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, &ColumnProxyModel::considerRowsAboutToBeMoved);
        connect(m_sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &ColumnProxyModel::considerRowsAboutToBeRemoved);
        connect(m_sourceModel, &QAbstractItemModel::rowsInserted, this, &ColumnProxyModel::considerRowsInserted);
        connect(m_sourceModel, &QAbstractItemModel::rowsMoved, this, &ColumnProxyModel::considerRowsMoved);
        connect(m_sourceModel, &QAbstractItemModel::rowsRemoved, this, &ColumnProxyModel::considerRowsRemoved);

        // TODO: QAbstractItemModel::modelAboutToBeReset and QAbstractItemModel::modelReset are private signals
        // we can connect to them, but can't emit them
        connect(m_sourceModel, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
        connect(m_sourceModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));

        connect(m_sourceModel, &QAbstractItemModel::headerDataChanged, this, &QAbstractItemModel::headerDataChanged);
        connect(m_sourceModel, &QAbstractItemModel::layoutAboutToBeChanged, this, &QAbstractItemModel::layoutAboutToBeChanged);
        connect(m_sourceModel, &QAbstractItemModel::layoutChanged, this, &QAbstractItemModel::layoutChanged);
    }
    endResetModel();
}

void ColumnProxyModel::setColumn(int col)
{
    beginResetModel();
    m_column = col;
    endResetModel();
}

int ColumnProxyModel::column() const
{
    return m_column;
}

QModelIndex ColumnProxyModel::rootIndex() const
{
    return m_index;
}

void ColumnProxyModel::setRootIndex(const QModelIndex &index)
{
    if (index == m_index) {
        return;
    }

    if (index.isValid()) {
        setSourceModel(const_cast<QAbstractItemModel *>(index.model()));
    }
    beginResetModel();
    m_index = index;
    endResetModel();

    Q_EMIT rootIndexChanged();
}

QModelIndex ColumnProxyModel::indexFromModel(QAbstractItemModel *model, int row, int column, const QModelIndex &parent)
{
    return model ? model->index(row, column, parent) : QModelIndex();
}

QVariant ColumnProxyModel::data(const QModelIndex &index, int role) const
{
    return m_sourceModel ? m_sourceModel->data(sourceIndex(index), role) : QVariant();
}

QVariant ColumnProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_sourceModel ? m_sourceModel->headerData(section, orientation, role) : QVariant();
}

QModelIndex ColumnProxyModel::sourceIndex(const QModelIndex &proxyIndex) const
{
    return m_sourceModel ? m_sourceModel->index(proxyIndex.row(), m_column, m_index) : QModelIndex();
}

int ColumnProxyModel::rowCount(const QModelIndex &parent) const
{
    return (!m_sourceModel || parent.isValid()) ? 0 : m_sourceModel->rowCount(m_index);
}

QModelIndex ColumnProxyModel::proxyIndex(const QModelIndex &sourceIndex) const
{
    if (sourceIndex.parent() == m_index) {
        return index(sourceIndex.row(), sourceIndex.column(), QModelIndex());
    }

    return QModelIndex();
}

void ColumnProxyModel::sourceDestroyed(QObject *source)
{
    Q_ASSERT(source == m_sourceModel);

    beginResetModel();
    m_sourceModel = nullptr;
    endResetModel();
}

QModelIndex ColumnProxyModel::indexAt(int row, const QModelIndex &parent) const
{
    return m_sourceModel ? m_sourceModel->index(row, m_column, parent) : QModelIndex();
}

/////////////////

void ColumnProxyModel::considerDataChanged(const QModelIndex &idxA, const QModelIndex &idxB)
{
    if (idxA.parent() == m_index && idxB.parent() == m_index) {
        Q_EMIT dataChanged(proxyIndex(idxA), proxyIndex(idxB));
    }
}

void ColumnProxyModel::considerRowsAboutToBeInserted(const QModelIndex &parent, int rA, int rB)
{
    if (parent == m_index) {
        beginInsertRows(QModelIndex(), rA, rB);
    }
}

void ColumnProxyModel::considerRowsAboutToBeMoved(const QModelIndex &sourceParent, int rA, int rB, const QModelIndex &destParent, int rD)
{
    if (sourceParent == m_index && destParent == m_index) {
        beginMoveRows(QModelIndex(), rA, rB, QModelIndex(), rD);
    } else if (sourceParent == m_index) {
        beginRemoveRows(sourceParent, rA, rB);
    } else if (destParent == m_index) {
        beginInsertRows(destParent, rD, rD + (rB - rA));
    }
}

void ColumnProxyModel::considerRowsAboutToBeRemoved(const QModelIndex &parent, int rA, int rB)
{
    if (parent == m_index) {
        beginRemoveRows(QModelIndex(), rA, rB);
    }
}

void ColumnProxyModel::considerRowsInserted(const QModelIndex &parent, int, int)
{
    if (parent == m_index) {
        endInsertRows();
    }
}

void ColumnProxyModel::considerRowsMoved(const QModelIndex &sourceParent, int, int, const QModelIndex &destParent, int)
{
    if (sourceParent == m_index && destParent == m_index) {
        endMoveRows();
    } else if (sourceParent == m_index) {
        endRemoveRows();
    } else if (destParent == m_index) {
        endInsertRows();
    }
}

void ColumnProxyModel::considerRowsRemoved(const QModelIndex &parent, int, int)
{
    if (parent == m_index) {
        endInsertRows();
    }
}

QHash<int, QByteArray> ColumnProxyModel::roleNames() const
{
    return m_sourceModel ? m_sourceModel->roleNames() : QHash<int, QByteArray>();
}

bool ColumnProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return m_sourceModel && m_sourceModel->setData(sourceIndex(index), value, role);
}
