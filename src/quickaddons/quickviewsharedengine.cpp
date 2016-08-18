/*
   This file is part of the KDE libraries

   Copyright (C) 2015 Marco Martin <mart@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

*/

#include "quickviewsharedengine.h"

#include <QTimer>
#include <QDebug>
#include <QUrl>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlExpression>
#include <QQmlProperty>

#include <klocalizedstring.h>
#include <kdeclarative/qmlobjectsharedengine.h>

#include <KPackage/Package>
#include <KPackage/PackageLoader>

namespace KQuickAddons {

class QuickViewSharedEnginePrivate
{
public:
    QuickViewSharedEnginePrivate(QuickViewSharedEngine *module)
        : q(module),
          resizeMode(QuickViewSharedEngine::ResizeMode::SizeRootObjectToView),
          initialSize(0, 0)
    {
        qmlObject = new KDeclarative::QmlObjectSharedEngine(q);
        QObject::connect(qmlObject, &KDeclarative::QmlObject::statusChanged,
                         q, &QuickViewSharedEngine::statusChanged);
        QObject::connect(qmlObject, SIGNAL(finished()),
                         q, SLOT(executionFinished()));
    }

    void executionFinished();
    void syncResizeMode();
    void syncWidth();
    void syncHeight();


    QuickViewSharedEngine *q;
    KDeclarative::QmlObjectSharedEngine *qmlObject;
    QuickViewSharedEngine::ResizeMode resizeMode;
    QSize initialSize;

};

void QuickViewSharedEnginePrivate::executionFinished()
{
    if (!qmlObject->rootObject()) {
        return;
    }

    QQuickItem *item = qobject_cast<QQuickItem *>(qmlObject->rootObject());

    if (!item) {
        return;
    }

    item->setParentItem(q->contentItem());
    initialSize = QSize(item->width(), item ->height());

    if(q->size().isEmpty()) {
        q->resize(initialSize);
        q->contentItem()->setWidth(item->width());
        q->contentItem()->setHeight(item->height());
    }

    syncResizeMode();
}

void QuickViewSharedEnginePrivate::syncResizeMode()
{
    QQuickItem *item = qobject_cast<QQuickItem *>(qmlObject->rootObject());

    if (!item) {
        return;
    }

    if (resizeMode == QuickViewSharedEngine::SizeRootObjectToView) {
        item->setWidth(q->width());
        item->setHeight(q->height());

        QObject::disconnect(item, SIGNAL(widthChanged()),
                            q, SLOT(syncWidth()));
        QObject::disconnect(item, SIGNAL(heightChanged()),
                            q, SLOT(syncHeight()));

    } else {

        QObject::connect(item, SIGNAL(widthChanged()),
                         q, SLOT(syncWidth()));
        QObject::connect(item, SIGNAL(heightChanged()),
                         q, SLOT(syncHeight()));

        syncWidth();
        syncHeight();
    }
}

void QuickViewSharedEnginePrivate::syncWidth()
{
    QQuickItem *item = qobject_cast<QQuickItem *>(qmlObject->rootObject());

    if (!item) {
        return;
    }

    q->setWidth(item->width());
}

void QuickViewSharedEnginePrivate::syncHeight()
{
    QQuickItem *item = qobject_cast<QQuickItem *>(qmlObject->rootObject());

    if (!item) {
        return;
    }

    q->setHeight(item->height());
}



QuickViewSharedEngine::QuickViewSharedEngine(QWindow *parent)
    : QQuickWindow(parent),
      d(new QuickViewSharedEnginePrivate(this))
{
}

QuickViewSharedEngine::~QuickViewSharedEngine()
{
}


void QuickViewSharedEngine::setTranslationDomain(const QString &translationDomain)
{
    d->qmlObject->setTranslationDomain(translationDomain);
}

QString QuickViewSharedEngine::translationDomain() const
{
    return d->qmlObject->translationDomain();
}

QQmlEngine *QuickViewSharedEngine::engine() const
{
    return d->qmlObject->engine();
}

QList<QQmlError> QuickViewSharedEngine::errors() const
{
    QList<QQmlError> errs;

    if (d->qmlObject->mainComponent()) {
        errs = d->qmlObject->mainComponent()->errors();
    }

    return errs;
}

QSize QuickViewSharedEngine::sizeHint() const
{
    QQuickItem *item = qobject_cast<QQuickItem *>(d->qmlObject->rootObject());
    if (!item) {
        return QSize();
    }

    const QSizeF implicitSize(item->implicitWidth(), item->implicitHeight());

    if (!implicitSize.isEmpty()) {
        return implicitSize.toSize();
    }

    if (item) {
        return QSize(item->width(), item->height());
    } else {
        return size();
    }
}

QSize QuickViewSharedEngine::initialSize() const
{
    return d->initialSize;
}

QuickViewSharedEngine::ResizeMode QuickViewSharedEngine::resizeMode() const
{
    return d->resizeMode;
}

QQmlContext *QuickViewSharedEngine::rootContext() const
{
    return d->qmlObject->rootContext();
}

QQuickItem *QuickViewSharedEngine::rootObject() const
{
     return qobject_cast<QQuickItem *>(d->qmlObject->rootObject());
}

void QuickViewSharedEngine::setResizeMode(ResizeMode mode)
{
    if (d->resizeMode == mode) {
        return;
    }

    d->resizeMode = mode;

    emit resizeModeChanged(mode);

    QQuickItem *item = qobject_cast<QQuickItem *>(d->qmlObject->rootObject());
    if (!item) {
        return;
    }

    d->syncResizeMode();
}

void QuickViewSharedEngine::setSource(const QUrl &url)
{
    if (d->qmlObject->source() == url) {
        return;
    }

    d->qmlObject->setSource(url);
    emit sourceChanged(url);
}

QUrl QuickViewSharedEngine::source() const
{
    return d->qmlObject->source();
}

QQmlComponent::Status QuickViewSharedEngine::status() const
{
    if (!d->qmlObject->mainComponent()) {
        return QQmlComponent::Null;
    }

    return QQmlComponent::Status(d->qmlObject->status());
}

void QuickViewSharedEngine::resizeEvent(QResizeEvent *e)
{
    QQuickItem *item = qobject_cast<QQuickItem *>(d->qmlObject->rootObject());
    if (item && d->resizeMode == SizeRootObjectToView) {
        item->setWidth(e->size().width());
        item->setHeight(e->size().height());
    }

    QQuickWindow::resizeEvent(e);
}

}

#include "moc_quickviewsharedengine.cpp"

