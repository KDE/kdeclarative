/**************************************************************************
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

#pragma once

#include <QPointer>
#include <QQuickItem>

class FallbackTapHandlerMouseEvent : public QObject {
    Q_OBJECT
    Q_PROPERTY(Qt::MouseButton button MEMBER m_button)
    Q_PROPERTY(Qt::MouseButtons buttons MEMBER m_buttons)
    Q_PROPERTY(Qt::KeyboardModifiers modifiers MEMBER m_modifiers)
    Q_PROPERTY(qreal x MEMBER m_x)
    Q_PROPERTY(qreal y MEMBER m_y)

public:
    FallbackTapHandlerMouseEvent(Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, qreal x, qreal y);
    ~FallbackTapHandlerMouseEvent() = default;

private:
    Qt::MouseButton m_button = Qt::NoButton;
    Qt::MouseButtons m_buttons = Qt::NoButton;
    Qt::KeyboardModifiers m_modifiers = Qt::NoModifier;
    qreal m_x = 0;
    qreal m_y = 0;
};

class FallbackTapHandler : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Qt::MouseButtons acceptedButtons READ acceptedMouseButtons WRITE setAcceptedButtons NOTIFY acceptedButtonsChanged)

public:
    explicit FallbackTapHandler(QQuickItem *parent = nullptr);
    ~FallbackTapHandler() override;

    void setAcceptedButtons(Qt::MouseButtons buttons);

Q_SIGNALS:
    void pressed(FallbackTapHandlerMouseEvent *event);
    void tapped(FallbackTapHandlerMouseEvent *event);
    void released(FallbackTapHandlerMouseEvent *event);
    void acceptedButtonsChanged();

protected:
    bool childMouseEventFilter(QQuickItem *item, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPointer<QQuickItem> m_mouseDownItem;
    QPointF m_mouseDownPos;
};

