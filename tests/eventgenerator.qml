/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
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
