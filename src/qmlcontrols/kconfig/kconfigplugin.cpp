/*
    SPDX-FileCopyrightText: 2016 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kconfigplugin.h"

#include <KAuthorized>
#include <QtQml>

#include "kauthorizedproxy.h"

void KConfigPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.kconfig"));

    qmlRegisterSingletonType<KAuthorizedProxy>(uri, 1, 0, "KAuthorized", [](QQmlEngine *, QJSEngine *) {
        auto proxy = new KAuthorizedProxy();

        // Register the enum values dynamically, just like one can access them in from KAuthorized namespace in C++
        const QMetaEnum restriction = QMetaEnum::fromType<KAuthorized::GenericRestriction>();
        for (int i = 0, count = restriction.keyCount(); i < count; ++i) {
            const auto key = restriction.key(i);
            proxy->setProperty(key, QString::fromLatin1(key).toLower());
        }
        const QMetaEnum action = QMetaEnum::fromType<KAuthorized::GenericAction>();
        for (int i = 0, count = action.keyCount(); i < count; ++i) {
            const auto key = action.key(i);
            proxy->setProperty(key, QString::fromLatin1(key).toLower());
        }
        return proxy;
    });
}
