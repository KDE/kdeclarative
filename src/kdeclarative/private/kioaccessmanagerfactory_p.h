/*
    SPDX-FileCopyrightText: 2010 Marco Martin <notmart@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KIOACCESSMANAGERFACTORY_H
#define KIOACCESSMANAGERFACTORY_H

#include <QQmlNetworkAccessManagerFactory>

namespace KDeclarative
{
class KIOAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    KIOAccessManagerFactory();
    ~KIOAccessManagerFactory();
    QNetworkAccessManager *create(QObject *parent) override;
};

}

#endif
