/***************************************************************************
 *   Copyright (C) 2015 by Eike Hein <hein@kde.org>
 *   Copyright (C) 2015 Marco Martin <notmart@gmail.com>
 *
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

#ifndef EventGenerator_H
#define EventGenerator_H

#include <QObject>

class QQuickItem;

class EventGenerator : public QObject
{
    Q_OBJECT

    public:
        enum MouseEvent {
            MouseButtonPress,
            MouseButtonRelease,
            MouseMove
        };
        Q_ENUM(MouseEvent)

        enum GrabEvent {
            GrabMouse,
            UngrabMouse
        };
        Q_ENUM(GrabEvent)

        EventGenerator(QObject *parent = nullptr);
        ~EventGenerator();

        /**
         * Send a mouse event of @type to the given @item
         */
        Q_INVOKABLE void sendMouseEvent(QQuickItem *item, EventGenerator::MouseEvent type, int x, int y, int button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);

        /**
         * Send a mouse event of @type to the given @item, all its children and descendants
         */
        Q_INVOKABLE void sendMouseEventRecursive(QQuickItem *item, EventGenerator::MouseEvent type, int x, int y, int button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);

        /**
         * Send a wheel event to the given @item
         *
         * @since 5.16
         */
        Q_INVOKABLE void sendWheelEvent(QQuickItem *item, int x, int y, const QPoint &pixelDelta, const QPoint &angleDelta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);

        /**
         * Send a wheel event to the given @item, all its children and descendants
         *
         * @since 5.16
         */
        Q_INVOKABLE void sendWheelEventRecursive(QQuickItem *item, int x, int y, const QPoint &pixelDelta, const QPoint &angleDelta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);

        /**
         * Send a mouse grab event of @type (grab or ungrab) to the given @item
         */
        Q_INVOKABLE void sendGrabEvent(QQuickItem *item, EventGenerator::GrabEvent type);

        /**
         * Send a mouse grab event of @type (grab or ungrab) to the given @item, all its children and descendants
         */
        Q_INVOKABLE void sendGrabEventRecursive(QQuickItem *item, EventGenerator::GrabEvent type);

    private:
        static QList<QQuickItem *> allChildItemsRecursive(QQuickItem *parentItem);
};

#endif
