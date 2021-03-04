/*
   SPDX-FileCopyrightText: 2010 Marco Martin <notmart@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kioaccessmanagerfactory_p.h"
#include <kio/accessmanager.h>

namespace KDeclarative
{
KIOAccessManagerFactory::KIOAccessManagerFactory()
    : QQmlNetworkAccessManagerFactory()
{
}

KIOAccessManagerFactory::~KIOAccessManagerFactory()
{
}

QNetworkAccessManager *KIOAccessManagerFactory::create(QObject *parent)
{
    return new KIO::AccessManager(parent);
}

}
