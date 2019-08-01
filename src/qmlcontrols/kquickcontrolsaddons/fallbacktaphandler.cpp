/***************************************************************************
 *   Copyright (C) 2019 by Marco Martin <mart@kde.org>                     *
 *   Copyright (C) 2014-2015 by Eike Hein <hein@kde.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "fallbacktaphandler.h"

#include <QGuiApplication>
#include <QStyleHints>

FallbackTapHandlerMouseEvent::FallbackTapHandlerMouseEvent(Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, qreal x, qreal y)
    : QObject(nullptr),
      m_button(button),
      m_buttons(buttons),
      m_modifiers(modifiers),
      m_x(x),
      m_y(y)
{}



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
    emit acceptedButtonsChanged();
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
        emit pressed(&eventProxy);

    } else if (event->type() == QEvent::MouseButtonRelease && item->inherits("QQuickFlickable")) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);

        if ((!(me->button() & acceptedMouseButtons()) && !(me->buttons() & acceptedMouseButtons())) ||
            m_mouseDownItem != item) {            
            return QQuickItem::childMouseEventFilter(item, event);
        }

        QPointF mappedPoint = mapFromItem(item, me->pos());
        FallbackTapHandlerMouseEvent eventProxy(me->button(), me->buttons(), me->modifiers(), mappedPoint.x(), mappedPoint.y());
        emit released(&eventProxy);

        if (QPointF(m_mouseDownPos - me->windowPos()).manhattanLength() < QGuiApplication::styleHints()->startDragDistance()) {
            emit tapped(&eventProxy);
        }

        m_mouseDownItem.clear();
        m_mouseDownPos = QPointF();
    }

    return QQuickItem::childMouseEventFilter(item, event);
}

void FallbackTapHandler::mousePressEvent(QMouseEvent *event)
{
    if ((!(event->button() & acceptedMouseButtons()) && !(event->buttons() & acceptedMouseButtons()))) {
        event->ignore();
        return;
    }

    event->accept();
    m_mouseDownItem = this;
    m_mouseDownPos = event->windowPos();

    FallbackTapHandlerMouseEvent eventProxy(event->button(), event->buttons(), event->modifiers(), event->x(), event->y());
    emit pressed(&eventProxy);
}

void FallbackTapHandler::mouseReleaseEvent(QMouseEvent *event)
{
    if ((!(event->button() & acceptedMouseButtons()) && !(event->buttons() & acceptedMouseButtons()))) {
        event->ignore();
        return;
    }

    FallbackTapHandlerMouseEvent eventProxy(event->button(), event->buttons(), event->modifiers(), event->x(), event->y());

    if (QPointF(m_mouseDownPos - event->windowPos()).manhattanLength() < QGuiApplication::styleHints()->startDragDistance()) {
        emit tapped(&eventProxy);
    }

    m_mouseDownItem.clear();
    m_mouseDownPos = QPointF();

    emit released(&eventProxy);
}

#include "moc_fallbacktaphandler.cpp"
