/*
    SPDX-FileCopyrightText: 2014 Antonis Tsiapaliokas <antonis.tsiapaliokas@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kioplugin.h"

#include <QtQml>

#include "krunproxy.h"

void KioPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.kio"));

    qmlRegisterType<KRunProxy>(uri, 1, 0, "KRun");
}
