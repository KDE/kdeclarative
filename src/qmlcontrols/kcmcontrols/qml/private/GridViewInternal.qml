/*
    SPDX-FileCopyrightText: 2019 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.3 as Kirigami


GridView {
    id: view

    property int implicitCellWidth: Kirigami.Units.gridUnit * 10
    property int implicitCellHeight: Math.round(implicitCellWidth / 1.6) + Kirigami.Units.gridUnit*3

    /**
     * Allow to highlight the selected item with Kirigami.Theme.neutralTextColor
     */
    property bool neutralHighlight: false

    onCurrentIndexChanged: positionViewAtIndex(currentIndex, GridView.Contain);

    QtObject {
        id: internal
        readonly property int availableWidth: scroll.width - internal.scrollBarSpace - 4
        readonly property int scrollBarSpace: scroll.QtControls.ScrollBar.vertical.width
    }
    anchors {
        fill: parent
        margins: 2
        leftMargin: scroll.QtControls.ScrollBar.vertical.visible ? 2 : Math.round(internal.scrollBarSpace/2) + 2
    }
    clip: true
    activeFocusOnTab: true

    Keys.onTabPressed: nextItemInFocusChain().forceActiveFocus(Qt.TabFocusReason)
    Keys.onBacktabPressed: nextItemInFocusChain(false).forceActiveFocus(Qt.TabFocusReason)

    cellWidth: Math.floor(internal.availableWidth / Math.max(Math.floor(internal.availableWidth / (implicitCellWidth + Kirigami.Units.gridUnit)), (Kirigami.Settings.isMobile ? 1 : 2)))
    cellHeight: Kirigami.Settings.isMobile ? cellWidth/1.6 + Kirigami.Units.gridUnit : implicitCellHeight

    keyNavigationEnabled: true
    keyNavigationWraps: true
    highlightMoveDuration: 0

    remove: Transition {
        ParallelAnimation {
            NumberAnimation { property: "scale"; to: 0.5; duration: Kirigami.Units.longDuration }
            NumberAnimation { property: "opacity"; to: 0.0; duration: Kirigami.Units.longDuration }
        }
    }

    removeDisplaced: Transition {
        SequentialAnimation {
            // wait for the "remove" animation to finish
            PauseAnimation { duration: Kirigami.Units.longDuration }
            NumberAnimation { properties: "x,y"; duration: Kirigami.Units.longDuration }
        }
    }
}
