/*
    SPDX-FileCopyrightText: 2012 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef COLUMNPROXYMODEL_H
#define COLUMNPROXYMODEL_H

#include <QAbstractListModel>

class ColumnProxyModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QModelIndex rootIndex READ rootIndex WRITE setRootIndex NOTIFY rootIndexChanged)
    //     Q_PROPERTY(QAbstractItemModel* sourceModel READ sourceModel WRITE setSourceModel) //rootIndex sets the model
    Q_PROPERTY(int column READ column WRITE setColumn)
public:
    explicit ColumnProxyModel(QObject *parent = nullptr);

    void setRootIndex(const QModelIndex &idx);
    QModelIndex rootIndex() const;

    void setSourceModel(QAbstractItemModel *sourceModel);
    QAbstractItemModel *sourceModel() const
    {
        return m_sourceModel;
    }

    int column() const;
    void setColumn(int col);

    Q_SCRIPTABLE static QModelIndex indexFromModel(QAbstractItemModel *model, int row, int column = 0, const QModelIndex &parent = QModelIndex());
    Q_SCRIPTABLE QModelIndex indexAt(int row, const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

Q_SIGNALS:
    void rootIndexChanged();

private:
    QModelIndex proxyIndex(const QModelIndex &sourceIndex) const;
    QModelIndex sourceIndex(const QModelIndex &proxyIndex) const;

    int m_column;
    QModelIndex m_index;
    QAbstractItemModel *m_sourceModel;

private Q_SLOTS:
    void considerRowsAboutToBeInserted(const QModelIndex &, int, int);
    void considerRowsAboutToBeMoved(const QModelIndex &sourceParent, int rA, int rB, const QModelIndex &destParent, int rD);
    void considerRowsAboutToBeRemoved(const QModelIndex &, int, int);
    void considerRowsRemoved(const QModelIndex &, int, int);
    void considerRowsMoved(const QModelIndex &, int, int, const QModelIndex &, int);
    void considerRowsInserted(const QModelIndex &, int, int);
    void considerDataChanged(const QModelIndex &idxA, const QModelIndex &idxB);
    void sourceDestroyed(QObject *source);
};

#endif
