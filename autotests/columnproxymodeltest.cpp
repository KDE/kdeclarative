/*
    SPDX-FileCopyrightText: 2012 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "columnproxymodeltest.h"
#include "../src/qmlcontrols/kquickcontrolsaddons/columnproxymodel.h"
#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QStandardItemModel>
#include <QTest>

QTEST_MAIN(ColumnProxyModelTest)

void ColumnProxyModelTest::testInit()
{
    qRegisterMetaType<QModelIndex>("QModelIndex");

    ColumnProxyModel *listify = new ColumnProxyModel;
    QSignalSpy spy(listify, &QAbstractItemModel::rowsInserted);

    new QAbstractItemModelTester(listify, listify);
    QStandardItemModel *m = new QStandardItemModel(listify);
    listify->setRootIndex(QModelIndex());
    listify->setSourceModel(m);
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    QStandardItem *item = new QStandardItem(QStringLiteral("lalalal"));
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    m->appendRow(item);
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));

    QCOMPARE(listify->rowCount(), 5);
    QCOMPARE(spy.count(), 5);

    ColumnProxyModel *listifyB = new ColumnProxyModel;
    new QAbstractItemModelTester(listifyB, listifyB);
    listifyB->setSourceModel(m);
    QCOMPARE(listifyB->rowCount(), 5);

    ColumnProxyModel *listifyC = new ColumnProxyModel;
    new QAbstractItemModelTester(listifyC, listifyC);
    listifyC->setRootIndex(item->index());
    QCOMPARE(listifyC->rowCount(), 3);

    delete listify;
    delete listifyB;
    delete listifyC;
}

void ColumnProxyModelTest::testSet()
{
    ColumnProxyModel listify;
    QSignalSpy spy(&listify, &QAbstractItemModel::rowsInserted);

    new QAbstractItemModelTester(&listify, &listify);
    QStandardItemModel *m = new QStandardItemModel(&listify);
    listify.setRootIndex(QModelIndex());
    listify.setSourceModel(m);
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    QStandardItem *item = new QStandardItem(QStringLiteral("lalalal"));
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    m->appendRow(item);
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));

    const QString newString = QStringLiteral("fu");
    QModelIndex changeIndex = listify.indexAt(0);
    listify.setData(changeIndex, QVariant::fromValue(newString), Qt::DisplayRole);
    QCOMPARE(changeIndex.data(Qt::DisplayRole).toString(), newString);
}
