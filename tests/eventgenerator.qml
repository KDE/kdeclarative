/*
 *   Copyright 2011 Marco Martin <mart@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.1
import org.kde.kquickcontrolsaddons 2.0

Rectangle {
    width: 500
    height: 500
    color: "red"

    EventGenerator {
        id: eventGenerator
    }

    MouseArea {
        id: parentMouseArea
        anchors.fill: parent
        drag.filterChildren: true

        onPressed: {
            print("Parent Mouse Pressed");
        }
        onPositionChanged: {
            print("Parent Position changed " + mouse.x + " " + mouse.y);
        }
        onReleased: {
            print("Parent Mouse Released");
        }

        onPressAndHold: {
            print("Parent press and hold");

            eventGenerator.sendGrabEvent(childMouseArea, EventGenerator.UngrabMouse);
            eventGenerator.sendGrabEvent(parentMouseArea, EventGenerator.GrabMouse);
            eventGenerator.sendMouseEvent(parentMouseArea, EventGenerator.MouseButtonPress, mouse.x, mouse.y, Qt.LeftButton, Qt.LeftButton, 0);

            print("Now Child Position changed events shouldn't appear anymore")
        }

        MouseArea {
            id: childMouseArea
            width: 300
            height: 300
            Rectangle {
                anchors.fill: parent
            }
            onPressed: print("Child Mouse Pressed");
            onPositionChanged: print("Child Position changed " + mouse.x + " " + mouse.y);
            onReleased: print("Child Mouse Released");
        }
    }
}
