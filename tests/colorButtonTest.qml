/*
 *   Copyright 2015 David Edmundson <davidedmundson@kde.org>
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
import org.kde.kquickcontrols 2.0

Rectangle {
    width: 300
    height: 300
    color: "white"

    ColorButton {
        id: btn

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10

        dialogTitle: "I am title"
        showAlphaChannel: true

        color: "blue" //a default colour
    }


    Rectangle {
        anchors.top: parent.top
        anchors.left: btn.right
        anchors.margins: 10

        width: 20
        height: 20

        color: btn.color
    }


}
