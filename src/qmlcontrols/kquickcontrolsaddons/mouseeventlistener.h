/*
    Copyright 2011 Marco Martin <notmart@gmail.com>
    Copyright 2013 Sebastian Kügler <sebas@kde.org>

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

#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include <QQuickItem>

/**
 * This item spies on mouse events from all child objects including child MouseAreas regardless
 * of whether the child MouseArea propegates events. It does not accept the event.
 *
 * In addition unlike MouseArea events include the mouse position in global co-ordinates and provides
 * the sceen the mouse is in.
 */


class KDeclarativeMouseEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(int screenX READ screenX)
    Q_PROPERTY(int screenY READ screenY)
    Q_PROPERTY(int button READ button)
    Q_PROPERTY(Qt::MouseButtons buttons READ buttons)
    Q_PROPERTY(Qt::KeyboardModifiers modifiers READ modifiers)
    Q_PROPERTY(QScreen* screen READ screen CONSTANT)

public:
    KDeclarativeMouseEvent(int x, int y, int screenX, int screenY,
                           Qt::MouseButton button,
                           Qt::MouseButtons buttons,
                           Qt::KeyboardModifiers modifiers,
                           QScreen* screen)
        : m_x(x),
          m_y(y),
          m_screenX(screenX),
          m_screenY(screenY),
          m_button(button),
          m_buttons(buttons),
          m_modifiers(modifiers),
          m_screen(screen)
    {}

    int x() const { return m_x; }
    int y() const { return m_y; }
    int screenX() const { return m_screenX; }
    int screenY() const { return m_screenY; }
    int button() const { return m_button; }
    Qt::MouseButtons buttons() const { return m_buttons; }
    Qt::KeyboardModifiers modifiers() const { return m_modifiers; }
    QScreen* screen() const { return m_screen; }

    // only for internal usage
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }

private:
    int m_x;
    int m_y;
    int m_screenX;
    int m_screenY;
    Qt::MouseButton m_button;
    Qt::MouseButtons m_buttons;
    Qt::KeyboardModifiers m_modifiers;
    QScreen *m_screen;
};

class KDeclarativeWheelEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x CONSTANT)
    Q_PROPERTY(int y READ y CONSTANT)
    Q_PROPERTY(int screenX READ screenX CONSTANT)
    Q_PROPERTY(int screenY READ screenY CONSTANT)
    Q_PROPERTY(int delta READ delta CONSTANT)
    Q_PROPERTY(Qt::MouseButtons buttons READ buttons CONSTANT)
    Q_PROPERTY(Qt::KeyboardModifiers modifiers READ modifiers CONSTANT)
    Q_PROPERTY(Qt::Orientation orientation READ orientation CONSTANT)

public:
    KDeclarativeWheelEvent(QPointF pos, QPoint screenPos, int delta,
                           Qt::MouseButtons buttons,
                           Qt::KeyboardModifiers modifiers,
                           Qt::Orientation orientation)
        : m_x(pos.x()),
          m_y(pos.y()),
          m_screenX(screenPos.x()),
          m_screenY(screenPos.y()),
          m_delta(delta),
          m_buttons(buttons),
          m_modifiers(modifiers),
          m_orientation(orientation)
    {}

    int x() const { return m_x; }
    int y() const { return m_y; }
    int screenX() const { return m_screenX; }
    int screenY() const { return m_screenY; }
    int delta() const { return m_delta; }
    Qt::MouseButtons buttons() const { return m_buttons; }
    Qt::KeyboardModifiers modifiers() const { return m_modifiers; }
    Qt::Orientation orientation() { return m_orientation; }

    // only for internal usage
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }

private:
    int m_x;
    int m_y;
    int m_screenX;
    int m_screenY;
    int m_delta;
    Qt::MouseButtons m_buttons;
    Qt::KeyboardModifiers m_modifiers;
    Qt::Orientation m_orientation;
};

class MouseEventListener : public QQuickItem
{
    Q_OBJECT
    /**
     * This property holds whether hover events are handled.
     * By default hover events are disabled
     */
    Q_PROPERTY(bool hoverEnabled READ hoverEnabled WRITE setHoverEnabled NOTIFY hoverEnabledChanged)

    /**
     * True if this MouseEventListener or any of its children contains the mouse cursor: this property will change only when the mouse button is pressed if hoverEnabled is false
     */
    Q_PROPERTY(bool containsMouse READ containsMouse NOTIFY containsMouseChanged)

    Q_PROPERTY(Qt::MouseButtons acceptedButtons READ acceptedButtons WRITE setAcceptedButtons NOTIFY acceptedButtonsChanged)

    /**
     * This property holds the cursor shape for this mouse area.
     * Note that on platforms that do not display a mouse cursor this may have no effect.
     */
    Q_PROPERTY(Qt::CursorShape cursorShape READ cursorShape WRITE setCursorShape RESET unsetCursor NOTIFY cursorShapeChanged)

    /**
     * True if the mouse is pressed in the item or any of its children
     */
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)

public:
    MouseEventListener(QQuickItem *parent=0);
    ~MouseEventListener();

    bool containsMouse() const;
    void setHoverEnabled(bool enable);
    bool hoverEnabled() const;
    bool isPressed() const;

    Qt::MouseButtons acceptedButtons() const;
    void setAcceptedButtons(Qt::MouseButtons buttons);

    Qt::CursorShape cursorShape() const;
    void setCursorShape(Qt::CursorShape shape);

protected:
    void hoverEnterEvent(QHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QHoverEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    bool childMouseEventFilter(QQuickItem *item, QEvent *event) Q_DECL_OVERRIDE;
    void mouseUngrabEvent() Q_DECL_OVERRIDE;
    void touchUngrabEvent() Q_DECL_OVERRIDE;

Q_SIGNALS:
    void pressed(KDeclarativeMouseEvent *mouse);
    void positionChanged(KDeclarativeMouseEvent *mouse);
    void released(KDeclarativeMouseEvent *mouse);
    void clicked(KDeclarativeMouseEvent *mouse);
    void pressAndHold(KDeclarativeMouseEvent *mouse);
    void wheelMoved(KDeclarativeWheelEvent *wheel);
    void containsMouseChanged(bool containsMouseChanged);
    void hoverEnabledChanged(bool hoverEnabled);
    void acceptedButtonsChanged();
    void cursorShapeChanged();
    void pressedChanged();
    void canceled();

private Q_SLOTS:
    void handlePressAndHold();
    void handleUngrab();

private:
    static QScreen* screenForGlobalPos(const QPoint &globalPos);

    bool m_pressed;
    KDeclarativeMouseEvent* m_pressAndHoldEvent;
    QPointF m_buttonDownPos;
    //Important: used only for comparison. If you will ever need to access this pointer, make it a QWeakPointer
    QEvent *m_lastEvent;
    QTimer *m_pressAndHoldTimer;
    bool m_containsMouse;
    Qt::MouseButtons m_acceptedButtons;
};

#endif
