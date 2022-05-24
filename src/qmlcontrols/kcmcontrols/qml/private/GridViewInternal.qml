/*
    SPDX-FileCopyrightText: 2019 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.15
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.19 as Kirigami


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
        leftMargin: 2 + (scroll.QtControls.ScrollBar.vertical.visible ? 0 : Math.round(internal.scrollBarSpace / 2))
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

    Loader {
        anchors.centerIn: parent
        width: parent.width - (Kirigami.Units.gridUnit * 8)
        active: parent.count === 0 && !startupTimer.running
        opacity: active && status === Loader.Ready ? 1 : 0
        visible: opacity > 0
        Behavior on opacity {
            OpacityAnimator {
                duration: Kirigami.Units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
        sourceComponent: Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            icon.name: "edit-none"
            text: i18n("No items found")
        }
    }

    // The view can take a bit of time to initialize itself when the KCM first
    // loads, during which time count is 0, which would cause the placeholder
    // message to appear for a moment and then disappear. To prevent this, let's
    // suppress it appearing for a moment after the KCM loads.
    Timer {
        id: startupTimer
        interval: Kirigami.Units.humanMoment
        running: false
    }
    Component.onCompleted: {
        startupTimer.start()
    }
}
