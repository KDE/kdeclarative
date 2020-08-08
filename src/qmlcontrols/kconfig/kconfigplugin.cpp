/*
    SPDX-FileCopyrightText: 2016 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kconfigplugin.h"

#include <QtQml>

#include "kauthorizedproxy.h"

QObject *kauthorizedProxy_singleton_provider(QQmlEngine *, QJSEngine *)
{
    return new KAuthorizedProxy;
}

void KConfigPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.kconfig"));

    qmlRegisterSingletonType<KAuthorizedProxy>(uri, 1, 0, "KAuthorized", kauthorizedProxy_singleton_provider);
}
