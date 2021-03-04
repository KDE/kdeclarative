/*
    SPDX-FileCopyrightText: 2012 Aleix Pol Gonzalez <aleixpol@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef COLUMNPROXYMODELTEST_H
#define COLUMNPROXYMODELTEST_H

#include <QObject>

class ColumnProxyModelTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testInit();
    void testSet();
};

#endif
