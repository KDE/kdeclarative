/*
 * Copyright 2016 Kai Uwe Broulik <kde@privat.broulik.de>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
