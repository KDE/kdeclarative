/*
    SPDX-FileCopyrightText: 2016 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kauthorizedproxy.h"

#include <KAuthorized>

KAuthorizedProxy::KAuthorizedProxy(QObject *parent)
    : QQmlPropertyMap(this, parent)
{
}

KAuthorizedProxy::~KAuthorizedProxy()
{
}

bool KAuthorizedProxy::authorize(const QString &action) const
{
    return KAuthorized::authorize(action);
}

bool KAuthorizedProxy::authorizeAction(const QString &action) const
{
    return KAuthorized::authorizeAction(action);
}
