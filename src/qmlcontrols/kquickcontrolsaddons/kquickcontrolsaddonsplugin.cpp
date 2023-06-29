/*
    SPDX-FileCopyrightText: 2009 Alan Alpert <alan.alpert@nokia.com>
    SPDX-FileCopyrightText: 2010 Ménard Alexis <menard@kde.org>
    SPDX-FileCopyrightText: 2010 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kquickcontrolsaddonsplugin.h"

#include "clipboard.h"
#include "columnproxymodel.h"
#include "eventgenerator.h"
#include "fallbacktaphandler.h"
#include "mimedatabase.h"
#include "mouseeventlistener.h"
#include "qimageitem.h"
#include "qpixmapitem.h"

void KQuickControlsAddonsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.kquickcontrolsaddons"));

    qmlRegisterType<QPixmapItem>(uri, 2, 0, "QPixmapItem");
    qmlRegisterType<QImageItem>(uri, 2, 0, "QImageItem");
    qmlRegisterType<MouseEventListener>(uri, 2, 0, "MouseEventListener");
    qmlRegisterAnonymousType<KDeclarativeMouseEvent>(uri, 1);
    qmlRegisterAnonymousType<KDeclarativeWheelEvent>(uri, 1);
    qmlRegisterType<ColumnProxyModel>(uri, 2, 0, "ColumnProxyModel");
    qmlRegisterType<Clipboard>(uri, 2, 0, "Clipboard");
    qmlRegisterType<MimeDatabase>(uri, 2, 0, "MimeDatabase");
    qmlRegisterType<EventGenerator>(uri, 2, 0, "EventGenerator");
    qmlRegisterUncreatableType<FallbackTapHandlerMouseEvent>(uri,
                                                             2,
                                                             1,
                                                             "FallbackTapHandlerMouseEvent",
                                                             QStringLiteral("Cannot create items of type FallbackTapHandlerMouseEvent"));
    qmlRegisterType<FallbackTapHandler>(uri, 2, 1, "FallbackTapHandler");

    qmlRegisterAnonymousType<QAbstractItemModel>(uri, 1);
    qRegisterMetaType<QModelIndex>("QModelIndex");
}

#include "moc_kquickcontrolsaddonsplugin.cpp"
