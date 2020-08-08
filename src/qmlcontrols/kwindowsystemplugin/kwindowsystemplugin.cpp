/*
    SPDX-FileCopyrightText: 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kwindowsystemplugin.h"

#include <QtQml>

#include "kwindowsystemproxy.h"

void KWindowSystemPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.kwindowsystem"));

    qmlRegisterType<KWindowSystemProxy>(uri, 1, 0, "KWindowSystem");
}
