/*
 * Copyright 2012 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */
#include "columnproxymodeltest.h"
#include "../src/qmlcontrols/kquickcontrolsaddons/columnproxymodel.h"
#include "modeltest.h"
#include <qtest.h>
#include <QSignalSpy>
#include <QStandardItemModel>

QTEST_MAIN(ColumnProxyModelTest)

void ColumnProxyModelTest::testInit()
{
    qRegisterMetaType<QModelIndex>("QModelIndex");
    
    ColumnProxyModel* listify = new ColumnProxyModel;
    QSignalSpy spy(listify, SIGNAL(rowsInserted(QModelIndex, int, int)));
    
    new ModelTest(listify, listify);
    QStandardItemModel* m = new QStandardItemModel(listify);
    listify->setRootIndex(QModelIndex());
    listify->setSourceModel(m);
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    QStandardItem* item = new QStandardItem(QStringLiteral("lalalal"));
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    m->appendRow(item);
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    
    QCOMPARE(listify->rowCount(), 5);
    QCOMPARE(spy.count(), 5);
    
    ColumnProxyModel* listifyB = new ColumnProxyModel;
    new ModelTest(listifyB, listifyB);
    listifyB->setSourceModel(m);
    QCOMPARE(listifyB->rowCount(), 5);
    
    ColumnProxyModel* listifyC = new ColumnProxyModel;
    new ModelTest(listifyC, listifyC);
    listifyC->setRootIndex(item->index());
    QCOMPARE(listifyC->rowCount(), 3);
    
    delete listify;
    delete listifyB;
    delete listifyC;
}

void ColumnProxyModelTest::testSet()
{
    ColumnProxyModel* listify = new ColumnProxyModel;
    QSignalSpy spy(listify, SIGNAL(rowsInserted(QModelIndex, int, int)));

    new ModelTest(listify, listify);
    QStandardItemModel* m = new QStandardItemModel(listify);
    listify->setRootIndex(QModelIndex());
    listify->setSourceModel(m);
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    m->appendRow(new QStandardItem(QStringLiteral("lalalal")));
    QStandardItem* item = new QStandardItem(QStringLiteral("lalalal"));
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));
    m->appendRow(item);
    item->appendRow(new QStandardItem(QStringLiteral("lelele")));

    const QString newString = QStringLiteral("fu");
    QModelIndex changeIndex = listify->indexAt(0);
    listify->setData(changeIndex, QVariant::fromValue(newString), Qt::DisplayRole);
    QCOMPARE(changeIndex.data(Qt::DisplayRole).toString(), newString);
}
