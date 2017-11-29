/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 * Copyright 2014  David Edmundson <davidedmundson@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "kcoreaddonsplugin.h"

#include <QtQml>

#include "formats.h"
#include "kuserproxy.h"

#include <KFormat>

static QObject *formats_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new Formats();
}

void KCoreAddonsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.kcoreaddons"));

    qmlRegisterSingletonType<Formats>(uri, 1, 0, "Format", formats_singletontype_provider);
    qRegisterMetaType<QLocale::FormatType>();

    qmlRegisterUncreatableType<KFormat>(uri, 1, 0, "FormatTypes", QString());

    qmlRegisterType<KUserProxy>(uri, 1, 0, "KUser");
}
