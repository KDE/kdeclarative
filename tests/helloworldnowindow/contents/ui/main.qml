/*
    SPDX-FileCopyrightText: 2014 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    implicitWidth: 300
    implicitHeight: 100
    visible: true

    Column {
        Label {
            text: i18n("Hello world")
        }
        Label {
            text: commandlineArguments.join(" ")
        }
    }
}
