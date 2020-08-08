/*
    SPDX-FileCopyrightText: 2014 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

ApplicationWindow {
    width: 300
    height: 100
    visible: true

    menuBar: MenuBar {
        Menu {
            title: i18n("Menu")
            MenuItem {
                text: i18n("item1")
            }
            MenuItem {
                text: i18n("item1")
            }
        }
    }

    toolBar: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: i18n("Button")
            }
        }
    }

    Column {
        Label {
            text: i18n("Hello world")
        }
        Label {
            text: commandlineArguments.join(" ")
        }    
    }
}
