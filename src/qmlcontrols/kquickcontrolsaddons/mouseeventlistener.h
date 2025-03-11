/*
    SPDX-FileCopyrightText: 2011 Marco Martin <notmart@gmail.com>
    SPDX-FileCopyrightText: 2013 Sebastian Kügler <sebas@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include <QQuickItem>

/*!
 * \qmltype KDeclarativeMouseEvent
 * \inqmlmodule org.kde.kquickcontrols.addons
 *
 * \brief This item spies on mouse events from all child objects
 * including child MouseAreas regardless
 * of whether the child MouseArea propagates events.
 * It does not accept the event.
 *
 * In addition unlike MouseArea events include the mouse position
 * in global coordinates and provides
 * the screen the mouse is in.
 */
class KDeclarativeMouseEvent : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    /*! \qmlproperty bool KDeclarativeMouseEvent::x */
    Q_PROPERTY(int x READ x)
    /*! \qmlproperty bool KDeclarativeMouseEvent::y */
    Q_PROPERTY(int y READ y)
    /*! \qmlproperty bool KDeclarativeMouseEvent::screenX */
    Q_PROPERTY(int screenX READ screenX)
    /*! \qmlproperty bool KDeclarativeMouseEvent::screenY */
    Q_PROPERTY(int screenY READ screenY)
    /*! \qmlproperty bool KDeclarativeMouseEvent::button */
    Q_PROPERTY(int button READ button)
    /*! \qmlproperty Qt::MouseButtons KDeclarativeMouseEvent::buttons */
    Q_PROPERTY(Qt::MouseButtons buttons READ buttons)
    /*! \qmlproperty Qt::KeyboardModifiers KDeclarativeMouseEvent::modifiers */
    Q_PROPERTY(Qt::KeyboardModifiers modifiers READ modifiers)
    /*! \qmlproperty QScreen KDeclarativeMouseEvent::screen */
    Q_PROPERTY(QScreen *screen READ screen CONSTANT)
    /*! \qmlproperty bool KDeclarativeMouseEvent::accepted */
    Q_PROPERTY(bool accepted READ isAccepted WRITE setAccepted NOTIFY acceptedChanged)
    /*! \qmlproperty int KDeclarativeMouseEvent::source */
    Q_PROPERTY(int source READ source)

public:
    KDeclarativeMouseEvent(int x,
                           int y,
                           int screenX,
                           int screenY,
                           Qt::MouseButton button,
                           Qt::MouseButtons buttons,
                           Qt::KeyboardModifiers modifiers,
                           QScreen *screen,
                           Qt::MouseEventSource source)
        : m_x(x)
        , m_y(y)
        , m_screenX(screenX)
        , m_screenY(screenY)
        , m_button(button)
        , m_buttons(buttons)
        , m_modifiers(modifiers)
        , m_screen(screen)
        , m_source(source)
    {
    }

    int x() const
    {
        return m_x;
    }
    int y() const
    {
        return m_y;
    }
    int screenX() const
    {
        return m_screenX;
    }
    int screenY() const
    {
        return m_screenY;
    }
    int button() const
    {
        return m_button;
    }
    Qt::MouseButtons buttons() const
    {
        return m_buttons;
    }
    Qt::KeyboardModifiers modifiers() const
    {
        return m_modifiers;
    }
    QScreen *screen() const
    {
        return m_screen;
    }
    int source() const
    {
        return m_source;
    }

    bool isAccepted() const
    {
        return m_accepted;
    }
    void setAccepted(bool accepted)
    {
        if (m_accepted != accepted) {
            m_accepted = accepted;
            Q_EMIT acceptedChanged();
        }
    }

    // only for internal usage
    void setX(int x)
    {
        m_x = x;
    }
    void setY(int y)
    {
        m_y = y;
    }

Q_SIGNALS:
    /*! \qmlsignal KDeclarativeMouseEvent::acceptedChanged() */
    void acceptedChanged();

private:
    int m_x;
    int m_y;
    int m_screenX;
    int m_screenY;
    Qt::MouseButton m_button;
    Qt::MouseButtons m_buttons;
    Qt::KeyboardModifiers m_modifiers;
    QScreen *m_screen;
    bool m_accepted = false;
    int m_source;
};

/*!
 * \qmltype KDeclarativeWheelEvent
 * \inqmlmodule org.kde.kquickcontrols.addons
 */
class KDeclarativeWheelEvent : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    /*! \qmlproperty int KDeclarativeWheelEvent::x */
    Q_PROPERTY(int x READ x CONSTANT)
    /*! \qmlproperty int KDeclarativeWheelEvent::y */
    Q_PROPERTY(int y READ y CONSTANT)
    /*! \qmlproperty int KDeclarativeWheelEvent::screenX */
    Q_PROPERTY(int screenX READ screenX CONSTANT)
    /*! \qmlproperty int KDeclarativeWheelEvent::screenY */
    Q_PROPERTY(int screenY READ screenY CONSTANT)
    /*! \qmlproperty int KDeclarativeWheelEvent::deltaX */
    Q_PROPERTY(int deltaX READ deltaX CONSTANT)
    /*! \qmlproperty int KDeclarativeWheelEvent::deltaY */
    Q_PROPERTY(int deltaY READ deltaY CONSTANT)
    /*! \qmlproperty int KDeclarativeWheelEvent::delta
     *  \deprecated Use deltaY instead.
     */
    Q_PROPERTY(int delta READ deltaY CONSTANT)
    /*! \qmlproperty Qt::MouseButtons KDeclarativeWheelEvent::buttons */
    Q_PROPERTY(Qt::MouseButtons buttons READ buttons CONSTANT)
    /*! \qmlproperty Qt::KeyboardModifiers KDeclarativeWheelEvent::modifiers */
    Q_PROPERTY(Qt::KeyboardModifiers modifiers READ modifiers CONSTANT)
    /*! \qmlproperty Qt::Orientation KDeclarativeWheelEvent::orientation
     *  \deprecated
     */
    Q_PROPERTY(Qt::Orientation orientation READ orientation CONSTANT)

public:
    KDeclarativeWheelEvent(QPointF pos,
                           QPoint screenPos,
                           QPoint angleDelta,
                           Qt::MouseButtons buttons,
                           Qt::KeyboardModifiers modifiers,
                           Qt::Orientation orientation)
        : m_x(pos.x())
        , m_y(pos.y())
        , m_screenX(screenPos.x())
        , m_screenY(screenPos.y())
        , m_angleDelta(angleDelta)
        , m_buttons(buttons)
        , m_modifiers(modifiers)
        , m_orientation(orientation)
    {
    }

    int x() const
    {
        return m_x;
    }
    int y() const
    {
        return m_y;
    }
    int screenX() const
    {
        return m_screenX;
    }
    int screenY() const
    {
        return m_screenY;
    }
    int deltaX() const
    {
        return m_angleDelta.x();
    }
    int deltaY() const
    {
        return m_angleDelta.y();
    }
    Qt::MouseButtons buttons() const
    {
        return m_buttons;
    }
    Qt::KeyboardModifiers modifiers() const
    {
        return m_modifiers;
    }
    Qt::Orientation orientation()
    {
        return m_orientation;
    } // TODO KF6: remove

    // only for internal usage
    void setX(int x)
    {
        m_x = x;
    }
    void setY(int y)
    {
        m_y = y;
    }

private:
    int m_x;
    int m_y;
    int m_screenX;
    int m_screenY;
    QPoint m_angleDelta;
    Qt::MouseButtons m_buttons;
    Qt::KeyboardModifiers m_modifiers;
    Qt::Orientation m_orientation;
};

/*!
 * \qmltype MouseEventListener
 * \inqmlmodule org.kde.kquickcontrols.addons
 */
class MouseEventListener : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    /*!
     * \qmlproperty bool MouseEventListener::hoverEnabled
     * This property holds whether hover events are handled.
     * By default hover events are disabled
     */
    Q_PROPERTY(bool hoverEnabled READ hoverEnabled WRITE setHoverEnabled NOTIFY hoverEnabledChanged)

    /*!
     * \qmlproperty bool MouseEventListener::containsMouse
     * True if this MouseEventListener or any of its children contains the mouse cursor:
     * this property will change only when the mouse button is pressed if hoverEnabled is false.
     */
    Q_PROPERTY(bool containsMouse READ containsMouse NOTIFY containsMouseChanged)

    /*! \qmlproperty Qt::MouseButtons MouseEventListener::acceptedButtons */
    Q_PROPERTY(Qt::MouseButtons acceptedButtons READ acceptedButtons WRITE setAcceptedButtons NOTIFY acceptedButtonsChanged)

    /*!
     * \qmlproperty Qt::CursorShape MouseEventListener::cursorShape
     * This property holds the cursor shape for this mouse area.
     * Note that on platforms that do not display a mouse cursor this may have no effect.
     */
    Q_PROPERTY(Qt::CursorShape cursorShape READ cursorShape WRITE setCursorShape RESET unsetCursor NOTIFY cursorShapeChanged)

    /*!
     * \qmlproperty bool MouseEventListener::pressed
     * True if the mouse is pressed in the item or any of its children
     */
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)

public:
    MouseEventListener(QQuickItem *parent = nullptr);
    ~MouseEventListener() override;

    bool containsMouse() const;
    void setHoverEnabled(bool enable);
    bool hoverEnabled() const;
    bool isPressed() const;

    Qt::MouseButtons acceptedButtons() const;
    void setAcceptedButtons(Qt::MouseButtons buttons);

    Qt::CursorShape cursorShape() const;
    void setCursorShape(Qt::CursorShape shape);

protected:
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool childMouseEventFilter(QQuickItem *item, QEvent *event) override;
    void mouseUngrabEvent() override;
    void touchUngrabEvent() override;

Q_SIGNALS:
    /*! \qmlsignal MouseEventListener::pressed(KDeclarativeMouseEventmouse) */
    void pressed(KDeclarativeMouseEvent *mouse);
    /*! \qmlsignal MouseEventListener::positionChanged(KDeclarativeMouseEvent mouse) */
    void positionChanged(KDeclarativeMouseEvent *mouse);
    /*! \qmlsignal MouseEventListener::released(KDeclarativeMouseEvent mouse) */
    void released(KDeclarativeMouseEvent *mouse);
    /*! \qmlsignal MouseEventListener::clicked(KDeclarativeMouseEvent mouse) */
    void clicked(KDeclarativeMouseEvent *mouse);
    /*! \qmlsignal MouseEventListener::pressAndHold(KDeclarativeMouseEvent *mouse) */
    void pressAndHold(KDeclarativeMouseEvent *mouse);
    /*! \qmlsignal MouseEventListener::wheelMoved(KDeclarativeWheelEvent wheel) */
    void wheelMoved(KDeclarativeWheelEvent *wheel);
    /*! \qmlsignal MouseEventListener::containsMouseChanged(bool containsMouseChanged) */
    void containsMouseChanged(bool containsMouseChanged);
    /*! \qmlsignal MouseEventListener::hoverEnabledChanged(bool hoverEnabled) */
    void hoverEnabledChanged(bool hoverEnabled);
    /*! \qmlsignal MouseEventListener::acceptedButtonsChanged() */
    void acceptedButtonsChanged();
    /*! \qmlsignal MouseEventListener::cursorShapeChanged() */
    void cursorShapeChanged();
    /*! \qmlsignal MouseEventListener::pressedChanged() */
    void pressedChanged();
    /*! \qmlsignal MouseEventListener::canceled() */
    void canceled();

private Q_SLOTS:
    void handlePressAndHold();
    void handleUngrab();

private:
    static QScreen *screenForGlobalPos(const QPointF &globalPos);

    bool m_pressed;
    KDeclarativeMouseEvent *m_pressAndHoldEvent;
    QPointF m_buttonDownPos;
    // Important: used only for comparison. If you will ever need to access this pointer, make it a QWeakPointer
    QEvent *m_lastEvent;
    QTimer *m_pressAndHoldTimer;
    bool m_containsMouse = false;
    bool m_childContainsMouse = false;
    Qt::MouseButtons m_acceptedButtons;
};

#endif
