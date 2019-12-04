/*
 *   Copyright 2009 by Alan Alpert <alan.alpert@nokia.com>
 *   Copyright 2010 by Ménard Alexis <menard@kde.org>
 *   Copyright 2010 by Marco Martin <mart@kde.org>

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "kquickcontrolsaddonsplugin.h"
#include "config-kquickcontrolsaddons.h"


#include "qpixmapitem.h"
#include "qimageitem.h"
#include "qiconitem.h"
#include "mouseeventlistener.h"
#include "columnproxymodel.h"
#include "clipboard.h"
#include "fallbacktaphandler.h"
#include "mimedatabase.h"
#include "kcmshell.h"
#include "icondialog.h"
#include "eventgenerator.h"

#if HAVE_EPOXY
#include "plotter.h"
#endif

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
    qmlRegisterType<QIconItem>(uri, 2, 0, "QIconItem");
    qmlRegisterType<MouseEventListener>(uri, 2, 0, "MouseEventListener");
    qmlRegisterType<ColumnProxyModel>(uri, 2, 0, "ColumnProxyModel");
    qmlRegisterType<Clipboard>(uri, 2, 0, "Clipboard");
    qmlRegisterType<MimeDatabase>(uri, 2, 0, "MimeDatabase");
    qmlRegisterSingletonType<KCMShell>(uri, 2, 0, "KCMShell", kcmshell_singleton_provider);
    qmlRegisterType<IconDialog>(uri, 2, 0, "IconDialog");
    qmlRegisterType<EventGenerator>(uri, 2, 0, "EventGenerator");
    qmlRegisterUncreatableType<FallbackTapHandlerMouseEvent>(uri, 2, 1, "FallbackTapHandlerMouseEvent", QStringLiteral("Cannot create items of type FallbackTapHandlerMouseEvent"));
    qmlRegisterType<FallbackTapHandler>(uri, 2, 1, "FallbackTapHandler");

#if HAVE_EPOXY
    qmlRegisterType<PlotData>(uri, 2, 0, "PlotData");
    qmlRegisterType<Plotter>(uri, 2, 0, "Plotter");
#endif
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    qmlRegisterType<QAbstractItemModel>();
#else
    qmlRegisterAnonymousType<QAbstractItemModel>(uri, 1);
#endif
    qRegisterMetaType<QModelIndex>("QModelIndex");
}



