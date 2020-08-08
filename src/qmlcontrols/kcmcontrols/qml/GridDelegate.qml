/*
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only
*/

import QtQuick 2.8
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as Controls
import QtQuick.Templates 2.2 as T2
import QtGraphicalEffects 1.0

import org.kde.kirigami 2.12 as Kirigami

/**
 * Base delegate for KControlmodules based on Grid views of thumbnails
 * Use the onClicked signal handler for managing the main action when
 * the user clicks on the thumbnail
 * @inherits QtQuick.Templates.ItemDelegate
 */
T2.ItemDelegate {
    id: delegate

    /**
     * toolTip: string
     * string for a tooltip for the whole delegate
     */
    property string toolTip

    /**
     * subtitle: string
     * optional string for the text to show below the main label
     */
    property string subtitle

    /**
     * thumbnail: Item
     * the item actually implementing the thumbnail: the visualization is up to the implementation
     */
    property alias thumbnail: thumbnailArea.data

    /**
     * thumbnailAvailable: bool
     * Set it to true when a thumbnail is actually available: when false,
     * only an icon will be shown instead of the actual thumbnail
     * ("edit-none" if pluginName is "None", otherwise it uses "view-preview").
     */
    property bool thumbnailAvailable: false

    /**
     * actions: list<Action>
     * A list of extra actions for the thumbnails. They will be shown as
     * icons on the bottom-right corner of the thumbnail on mouse over
     */
    property list<QtObject> actions

    width: GridView.view.cellWidth
    height: GridView.view.cellHeight
    hoverEnabled: true

    Kirigami.ShadowedRectangle {
        id: thumbnail
        anchors {
           centerIn: parent
           verticalCenterOffset: Math.ceil(-labelLayout.height/2)
        }
        width: Kirigami.Settings.isMobile ? delegate.width - Kirigami.Units.gridUnit : Math.min(delegate.GridView.view.implicitCellWidth, delegate.width - Kirigami.Units.gridUnit)
        height: Kirigami.Settings.isMobile ? Math.round((delegate.width - Kirigami.Units.gridUnit) / 1.6)
                                           : Math.min(delegate.GridView.view.implicitCellHeight - Kirigami.Units.gridUnit * 3,
                                                      delegate.height - Kirigami.Units.gridUnit)
        radius: Kirigami.Units.smallSpacing
        Kirigami.Theme.inherit: false
        Kirigami.Theme.colorSet: Kirigami.Theme.View

        shadow.xOffset: 0
        shadow.yOffset: 2
        shadow.size: 10
        shadow.color: Qt.rgba(0, 0, 0, 0.3)

        color: {
            if (delegate.GridView.isCurrentItem) {
                if (delegate.GridView.view.neutralHighlight) {
                    return Kirigami.Theme.neutralTextColor;
                }
                return Kirigami.Theme.highlightColor;
            } else if (parent.hovered) {
                return Kirigami.Theme.highlightColor;
            } else {
                return Kirigami.Theme.backgroundColor;
            }
        }
        Behavior on color {
            ColorAnimation {
                duration: Kirigami.Units.longDuration
                easing.type: Easing.OutQuad
            }
        }

        Rectangle {
            id: thumbnailArea
            radius: Kirigami.Units.smallSpacing/2
            anchors {
                fill: parent
                margins: Kirigami.Units.smallSpacing
            }

            color: Kirigami.Theme.backgroundColor
            Kirigami.Icon {
                visible: !delegate.thumbnailAvailable
                anchors.centerIn: parent
                width: Kirigami.Units.iconSizes.large
                height: width
                source: typeof pluginName === "string" && pluginName === "None" ? "edit-none" : "view-preview"
            }
        }

        Rectangle {
            anchors.fill: thumbnailArea
            visible: actionsRow.children.length > 0
            opacity: Kirigami.Settings.isMobile || delegate.hovered || (actionsScope.focus) ? 1 : 0
            radius: delegate.thumbnailAvailable ? 0 : thumbnailArea.radius
            color: Kirigami.Settings.isMobile ? "transparent" : Qt.rgba(1, 1, 1, 0.2)

            Behavior on opacity {
                PropertyAnimation {
                    duration: Kirigami.Units.longDuration
                    easing.type: Easing.OutQuad
                }
            }

            FocusScope {
                id: actionsScope

                anchors {
                    right: parent.right
                    rightMargin: Kirigami.Units.smallSpacing
                    bottom: parent.bottom
                    bottomMargin: Kirigami.Units.smallSpacing
                }
                width: actionsRow.width
                height: actionsRow.height

                RowLayout {
                    id: actionsRow

                    Repeater {
                        model: delegate.actions
                        delegate: Controls.Button {
                            icon.name: modelData.iconName
                            activeFocusOnTab: focus || delegate.focus
                            onClicked: modelData.trigger()
                            enabled: modelData.enabled
                            visible: modelData.visible
                            //NOTE: there aren't any global settings where to take "official" tooltip timeouts
                            Controls.ToolTip.delay: 1000
                            Controls.ToolTip.timeout: 5000
                            Controls.ToolTip.visible: (Kirigami.Settings.isMobile ? pressed : hovered) && modelData.tooltip.length > 0
                            Controls.ToolTip.text: modelData.tooltip
                        }
                    }
                }
            }
        }
    }

    ColumnLayout {
        id: labelLayout
        spacing: 0
        anchors {
            left: thumbnail.left
            right: thumbnail.right
            top: thumbnail.bottom
            topMargin: caption.visible ? Kirigami.Units.smallSpacing : Kirigami.Units.largeSpacing
        }

        // FIXME: These labels are center-aligned with a maximum width instead
        // of the more conventional combination of "Layout.fillWidth: true"
        // and "horizontalAlignment: Text.AlignHCenter" because that combination
        // triggers https://bugreports.qt.io/browse/QTBUG-49646
        Controls.Label {
            Layout.alignment: Qt.AlignHCenter
            Layout.maximumWidth: labelLayout.width
            text: delegate.text
            elide: Text.ElideRight
        }
        Controls.Label {
            id: caption
            Layout.alignment: Qt.AlignHCenter
            Layout.maximumWidth: labelLayout.width
            visible: delegate.subtitle.length > 0
            opacity: 0.6
            text: delegate.subtitle
            font.pointSize: theme.smallestFont.pointSize
            elide: Text.ElideRight
        }
    }

    Controls.ToolTip.delay: 1000
    Controls.ToolTip.timeout: 5000
    Controls.ToolTip.visible: hovered && delegate.toolTip.length > 0
    Controls.ToolTip.text: toolTip
}
