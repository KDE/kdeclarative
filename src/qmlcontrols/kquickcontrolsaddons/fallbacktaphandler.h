/*
    SPDX-FileCopyrightText: 2019 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2014-2015 Eike Hein <hein@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QPointer>
#include <QQuickItem>

class FallbackTapHandlerMouseEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Qt::MouseButton button MEMBER m_button)
    Q_PROPERTY(Qt::MouseButtons buttons MEMBER m_buttons)
    Q_PROPERTY(Qt::KeyboardModifiers modifiers MEMBER m_modifiers)
    Q_PROPERTY(qreal x MEMBER m_x)
    Q_PROPERTY(qreal y MEMBER m_y)

public:
    FallbackTapHandlerMouseEvent(Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, qreal x, qreal y);
    ~FallbackTapHandlerMouseEvent() override = default;

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
