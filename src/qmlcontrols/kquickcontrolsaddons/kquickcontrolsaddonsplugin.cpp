/*
    SPDX-FileCopyrightText: 2009 Alan Alpert <alan.alpert@nokia.com>
    SPDX-FileCopyrightText: 2010 Ménard Alexis <menard@kde.org>
    SPDX-FileCopyrightText: 2010 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kquickcontrolsaddonsplugin.h"
#include "config-kquickcontrolsaddons.h"

#include "clipboard.h"
#include "columnproxymodel.h"
#include "eventgenerator.h"
#include "fallbacktaphandler.h"
#include "icondialog.h"
#include "kcmshell.h"
#include "mimedatabase.h"
#include "mouseeventlistener.h"
#include "qiconitem.h"
#include "qimageitem.h"
#include "qpixmapitem.h"

#if HAVE_EPOXY
#include "plotter.h"
#endif

#include "../../kdeclarative/kdeclarative_export.h"

static QObject *kcmshell_singleton_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return new KCMShell();
}

void KQuickControlsAddonsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.kquickcontrolsaddons"));

    qmlRegisterType<QPixmapItem>(uri, 2, 0, "QPixmapItem");
    qmlRegisterType<QImageItem>(uri, 2, 0, "QImageItem");
#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    qmlRegisterType<QIconItem>(uri, 2, 0, "QIconItem");
#endif
    qmlRegisterType<MouseEventListener>(uri, 2, 0, "MouseEventListener");
    qmlRegisterAnonymousType<KDeclarativeMouseEvent>(uri, 1);
    qmlRegisterAnonymousType<KDeclarativeWheelEvent>(uri, 1);
    qmlRegisterType<ColumnProxyModel>(uri, 2, 0, "ColumnProxyModel");
    qmlRegisterType<Clipboard>(uri, 2, 0, "Clipboard");
    qmlRegisterType<MimeDatabase>(uri, 2, 0, "MimeDatabase");
    qmlRegisterSingletonType<KCMShell>(uri, 2, 0, "KCMShell", kcmshell_singleton_provider);
    qmlRegisterType<IconDialog>(uri, 2, 0, "IconDialog");
    qmlRegisterType<EventGenerator>(uri, 2, 0, "EventGenerator");
    qmlRegisterUncreatableType<FallbackTapHandlerMouseEvent>(uri,
                                                             2,
                                                             1,
                                                             "FallbackTapHandlerMouseEvent",
                                                             QStringLiteral("Cannot create items of type FallbackTapHandlerMouseEvent"));
    qmlRegisterType<FallbackTapHandler>(uri, 2, 1, "FallbackTapHandler");

#if HAVE_EPOXY
    qmlRegisterType<PlotData>(uri, 2, 0, "PlotData");
    qmlRegisterType<Plotter>(uri, 2, 0, "Plotter");
#endif
    qmlRegisterAnonymousType<QAbstractItemModel>(uri, 1);
    qRegisterMetaType<QModelIndex>("QModelIndex");
}
