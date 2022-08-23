/*
    SPDX-FileCopyrightText: 2019 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2014-2015 Eike Hein <hein@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "fallbacktaphandler.h"

#include <QGuiApplication>
#include <QStyleHints>

FallbackTapHandlerMouseEvent::FallbackTapHandlerMouseEvent(Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, qreal x, qreal y)
    : QObject(nullptr)
    , m_button(button)
    , m_buttons(buttons)
    , m_modifiers(modifiers)
    , m_x(x)
    , m_y(y)
{
}

FallbackTapHandler::FallbackTapHandler(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFiltersChildMouseEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

FallbackTapHandler::~FallbackTapHandler()
{
}

void FallbackTapHandler::setAcceptedButtons(Qt::MouseButtons buttons)
{
    setAcceptedMouseButtons(buttons);
    Q_EMIT acceptedButtonsChanged();
}

bool FallbackTapHandler::childMouseEventFilter(QQuickItem *item, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress && item->inherits("QQuickFlickable")) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (!(me->button() & acceptedMouseButtons()) && !(me->buttons() & acceptedMouseButtons())) {
            return QQuickItem::childMouseEventFilter(item, event);
        }
        m_mouseDownItem = item;
        m_mouseDownPos = me->windowPos();

        QPointF mappedPoint = mapFromItem(item, me->pos());
        FallbackTapHandlerMouseEvent eventProxy(me->button(), me->buttons(), me->modifiers(), mappedPoint.x(), mappedPoint.y());
        Q_EMIT pressed(&eventProxy);

    } else if (event->type() == QEvent::MouseButtonRelease && item->inherits("QQuickFlickable")) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);

        if ((!(me->button() & acceptedMouseButtons()) && !(me->buttons() & acceptedMouseButtons())) || m_mouseDownItem != item) {
            return QQuickItem::childMouseEventFilter(item, event);
        }

        QPointF mappedPoint = mapFromItem(item, me->pos());
        FallbackTapHandlerMouseEvent eventProxy(me->button(), me->buttons(), me->modifiers(), mappedPoint.x(), mappedPoint.y());
        Q_EMIT released(&eventProxy);

        if (QPointF(m_mouseDownPos - me->windowPos()).manhattanLength() < QGuiApplication::styleHints()->startDragDistance()) {
            Q_EMIT tapped(&eventProxy);
        }

        m_mouseDownItem.clear();
        m_mouseDownPos = QPointF();
    }

    return QQuickItem::childMouseEventFilter(item, event);
}

void FallbackTapHandler::mousePressEvent(QMouseEvent *event)
{
    if (!(event->button() & acceptedMouseButtons()) && !(event->buttons() & acceptedMouseButtons())) {
        event->ignore();
        return;
    }

    event->accept();
    m_mouseDownItem = this;
    m_mouseDownPos = event->windowPos();

    FallbackTapHandlerMouseEvent eventProxy(event->button(), event->buttons(), event->modifiers(), event->x(), event->y());
    Q_EMIT pressed(&eventProxy);
}

void FallbackTapHandler::mouseReleaseEvent(QMouseEvent *event)
{
    if (!(event->button() & acceptedMouseButtons()) && !(event->buttons() & acceptedMouseButtons())) {
        event->ignore();
        return;
    }

    FallbackTapHandlerMouseEvent eventProxy(event->button(), event->buttons(), event->modifiers(), event->x(), event->y());

    if (QPointF(m_mouseDownPos - event->windowPos()).manhattanLength() < QGuiApplication::styleHints()->startDragDistance()) {
        Q_EMIT tapped(&eventProxy);
    }

    m_mouseDownItem.clear();
    m_mouseDownPos = QPointF();

    Q_EMIT released(&eventProxy);
}

#include "moc_fallbacktaphandler.cpp"
