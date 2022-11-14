/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "qiconitem.h"

#include <QIcon>
#include <QQuickWindow>
#include <quickaddons/imagetexturescache.h>
#include <quickaddons/managedtexturenode.h>

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)

Q_GLOBAL_STATIC(ImageTexturesCache, s_iconImageCache)

QIconItem::QIconItem(QQuickItem *parent)
    : QQuickItem(parent)
    , m_smooth(false)
    , m_state(DefaultState)
    , m_changed(false)
{
    setFlag(ItemHasContents, true);
}

QIconItem::~QIconItem()
{
}

void QIconItem::setIcon(const QVariant &icon)
{
    if (icon.canConvert<QIcon>()) {
        m_icon = icon.value<QIcon>();
    } else if (icon.canConvert<QString>()) {
        m_icon = QIcon::fromTheme(icon.toString());
    } else {
        m_icon = QIcon();
    }
    m_changed = true;
    update();
    Q_EMIT iconChanged();
}

QIcon QIconItem::icon() const
{
    return m_icon;
}

QIconItem::State QIconItem::state() const
{
    return m_state;
}

void QIconItem::setState(QIconItem::State state)
{
    if (m_state == state) {
        return;
    }

    m_state = state;
    m_changed = true;
    Q_EMIT stateChanged(state);
    update();
}

bool QIconItem::enabled() const
{
    return (m_state == DefaultState);
}

void QIconItem::setEnabled(bool enabled)
{
    if (enabled) {
        setState(DefaultState);
    } else {
        setState(DisabledState);
    }
}

int QIconItem::implicitWidth() const
{
    return 32;
}

int QIconItem::implicitHeight() const
{
    return 32;
}

void QIconItem::setSmooth(const bool smooth)
{
    if (smooth == m_smooth) {
        return;
    }
    m_smooth = smooth;
    m_changed = true;
    update();
    Q_EMIT smoothChanged();
}

bool QIconItem::smooth() const
{
    return m_smooth;
}

QSGNode *QIconItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData * /*data*/)
{
    if (m_icon.isNull()) {
        delete node;
        return nullptr;
    }

    if (m_changed || node == nullptr) {
        m_changed = false;

        ManagedTextureNode *mNode = dynamic_cast<ManagedTextureNode *>(node);
        if (!mNode) {
            delete node;
            mNode = new ManagedTextureNode;
        }

        QIcon::Mode mode;
        switch (m_state) {
        case DefaultState:
            mode = QIcon::Normal;
            break;
        case ActiveState:
            mode = QIcon::Active;
            break;
        case DisabledState:
            mode = QIcon::Disabled;
            break;
        case SelectedState:
            mode = QIcon::Selected;
            break;
        }

        QImage img;
        const QSize size(width(), height());
        if (!size.isEmpty()) {
            img = m_icon.pixmap(size, mode, QIcon::On).toImage();
        }
        mNode->setTexture(s_iconImageCache->loadTexture(window(), img));
        mNode->setRect(QRect(QPoint(0, 0), size));
        node = mNode;
    }

    return node;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void QIconItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
#else
void QIconItem::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
#endif
{
    if (newGeometry.size() != oldGeometry.size()) {
        m_changed = true;
        update();
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
#else
    QQuickItem::geometryChange(newGeometry, oldGeometry);
#endif
}
#endif
