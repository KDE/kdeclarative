/*
    SPDX-FileCopyrightText: 2010 BetterInbox <contact@betterinbox.com>
    SPDX-FileContributor: Gregory Schlomoff <greg@betterinbox.com>

    SPDX-License-Identifier: MIT
*/

#ifndef DECLARATIVEDRAGDROPEVENT_H
#define DECLARATIVEDRAGDROPEVENT_H

#include "DeclarativeDropArea.h"
#include "DeclarativeMimeData.h"
#include <QObject>

/*!
 * \qmltype DragDropEvent
 * \inqmlmodule org.kde.draganddrop
 */
class DeclarativeDragDropEvent : public QObject
{
    Q_OBJECT

    /*!
     * \qmlproperty int DragDropEvent::x
     * The mouse X position of the event relative to the DropArea that is receiving the event.
     */
    Q_PROPERTY(int x READ x)

    /*!
     * \qmlproperty int DragDropEvent::y
     * The mouse Y position of the event relative to the DropArea that is receiving the event.
     */
    Q_PROPERTY(int y READ y)

    /*!
     * \qmlproperty int DragDropEvent::buttons
     * The pressed mouse buttons.
     * A combination of:
     * \value Qt.NoButton    The button state does not refer to any button (see QMouseEvent::button()).
     * \value Qt.LeftButton    The left button is pressed, or an event refers to the left button. (The left button may be the right button on left-handed mice.)
     * \value Qt.RightButton    The right button.
     * \value Qt.MidButton    The middle button.
     * \value Qt.MiddleButton  MidButton  The middle button.
     * \value Qt.XButton1    The first X button.
     * \value Qt.XButton2    The second X button.
     */
    Q_PROPERTY(int buttons READ buttons)

    /*!
     * \qmlproperty int DragDropEvent::modifiers
     * Pressed keyboard modifiers, a combination of:
     * \value Qt.NoModifier    No modifier key is pressed.
     * \value Qt.ShiftModifier    A Shift key on the keyboard is pressed.
     * \value Qt.ControlModifier    A Ctrl key on the keyboard is pressed.
     * \value Qt.AltModifier    An Alt key on the keyboard is pressed.
     * \value Qt.MetaModifier    A Meta key on the keyboard is pressed.
     * \value Qt.KeypadModifier    A keypad button is pressed.
     * \value Qt.GroupSwitchModifier    X11 only. A Mode_switch key on the keyboard is pressed.
     */
    Q_PROPERTY(int modifiers READ modifiers)

    /*!
     * \qmlproperty MimeData DragDropEvent::mimeData
     * The mime data of this operation
     * \sa MimeData
     */
    Q_PROPERTY(DeclarativeMimeData *mimeData READ mimeData)

    /*!
     * \qmlproperty Qt::DropActions DragDropEvent::possibleActions
     * The possible kinds of action that can be done in the drop,
     * a combination of:
     * \value Qt.CopyAction
     *        0x1  Copy the data to the target.
     * \value Qt.MoveAction
     *        0x2  Move the data from the source to the target.
     * \value Qt.LinkAction
     *        0x4  Create a link from the source to the target.
     * \value Qt.ActionMask
     *        0xff
     * \value Qt.IgnoreAction
     *        0x0  Ignore the action (do nothing with the data).
     * \value Qt.TargetMoveAction
     *        0x8002  On Windows, this value is used when the ownership of
     *        the D&D data should be taken over by the target application,
     *        i.e., the source application should not delete the data.
     *        On X11 this value is used to do a move.
     *        TargetMoveAction is not used on the Mac.
     */
    Q_PROPERTY(Qt::DropActions possibleActions READ possibleActions)

    /*!
     * \qmlproperty Qt::DropAction DragDropEvent::proposedAction
     * Default action
     * \sa possibleActions
     */
    Q_PROPERTY(Qt::DropAction proposedAction READ proposedAction)

public:
    DeclarativeDragDropEvent(QDropEvent *e, DeclarativeDropArea *parent = nullptr);
    DeclarativeDragDropEvent(QDragLeaveEvent *e, DeclarativeDropArea *parent = nullptr);

    int x() const
    {
        return m_x;
    }
    int y() const
    {
        return m_y;
    }
    int buttons() const
    {
        return m_buttons;
    }
    int modifiers() const
    {
        return m_modifiers;
    }
    DeclarativeMimeData *mimeData();
    Qt::DropAction proposedAction() const
    {
        return m_event->proposedAction();
    }
    Qt::DropActions possibleActions() const
    {
        return m_event->possibleActions();
    }

public Q_SLOTS:
    /*!
     * \qmlmethod DragDropEvent::accept(int action)
     */
    void accept(int action);

    /*!
     * \qmlmethod DragDropEvent::ignore()
     */
    void ignore();

private:
    int m_x;
    int m_y;
    Qt::MouseButtons m_buttons;
    Qt::KeyboardModifiers m_modifiers;
    QScopedPointer<DeclarativeMimeData> m_data;
    QDropEvent *m_event;
};

#endif // DECLARATIVEDRAGDROPEVENT_H
