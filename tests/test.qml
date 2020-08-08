/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.1

Rectangle {
    width: 300
    height: 300
    color: "red"

    Text {
        anchors.fill: parent
        text: i18n("Hello")
    }
}
