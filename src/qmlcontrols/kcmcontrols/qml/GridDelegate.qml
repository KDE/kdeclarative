/*
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.8
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Templates 2.2 as T2
import QtGraphicalEffects 1.0

import org.kde.kirigami 2.12 as Kirigami

/**
 * Base delegate for KControlmodules based on Grid views of thumbnails
 * Use the onClicked signal handler for managing the main action when
 * the user clicks on the thumbnail
 * @inherits QtQuick.Templates.ItemDelegate
 */

// TODO(KF6): this is unbelievably fragile and changes can often lead to freezes and crashes, as well as visually unpleasant
// effects. Needs to be thrown out and done from scratch, because this is not conducive to making changes.
// Also see: private/GridViewInternal.qml.
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
    hoverEnabled: !Kirigami.Settings.isMobile

    Kirigami.AbstractCard {
        id: thumbnail

        Kirigami.Theme.inherit: false
        Kirigami.Theme.colorSet: Kirigami.Theme.View

        anchors.centerIn: parent

        topPadding: Kirigami.Units.largeSpacing
        bottomPadding: Kirigami.Units.largeSpacing

        contentItem: ColumnLayout {
            spacing: 0

            QQC2.Label {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                text: delegate.text
                elide: Text.ElideRight
                font.bold: delegate.GridView.isCurrentItem
            }
            QQC2.Label {
                id: caption
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                visible: delegate.subtitle.length > 0
                opacity: 0.6
                text: delegate.subtitle
                font.pointSize: theme.smallestFont.pointSize
                font.bold: delegate.GridView.isCurrentItem
                elide: Text.ElideRight
            }
            Rectangle {
                id: thumbnailArea
                radius: Kirigami.Units.smallSpacing/2

                implicitWidth: Kirigami.Settings.isMobile ? delegate.width - Kirigami.Units.gridUnit : Math.min(delegate.GridView.view.implicitCellWidth, delegate.width - Kirigami.Units.gridUnit)
                implicitHeight: Kirigami.Settings.isMobile ? Math.round((delegate.width - Kirigami.Units.gridUnit) / 1.6)
                                                : Math.min(delegate.GridView.view.implicitCellHeight - Kirigami.Units.gridUnit * 3,
                                                            (delegate.height) - Kirigami.Units.gridUnit)

                color: Kirigami.Theme.backgroundColor

                // "None/There's nothing here" indicator
                Kirigami.Icon {
                    visible: !delegate.thumbnailAvailable
                    anchors.centerIn: parent
                    width: Kirigami.Units.iconSizes.large
                    height: width
                    source: typeof pluginName === "string" && pluginName === "None" ? "edit-none" : "view-preview"
                }
            }
            Kirigami.Separator {
                Layout.fillWidth: true
                visible: actionToolBar.implicitHeight > 0
            }
            Kirigami.ActionToolBar {
                id: actionToolBar
                actions: delegate.actions
            }
        }
    }

    QQC2.ToolTip.delay: 1000
    QQC2.ToolTip.timeout: 5000
    QQC2.ToolTip.visible: hovered && delegate.toolTip.length > 0
    QQC2.ToolTip.text: toolTip
}
