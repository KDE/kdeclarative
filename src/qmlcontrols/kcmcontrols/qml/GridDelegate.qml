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

    onClicked: {
        GridView.currentIndex = index;
    }

    Kirigami.AbstractCard {
        leftPadding: Kirigami.Units.smallSpacing
        rightPadding: Kirigami.Units.smallSpacing
        topPadding: 0
        bottomPadding: 0

        anchors.centerIn: parent

        showClickFeedback: true
        highlighted: delegate.GridView.isCurrentItem
        onClicked: {
            delegate.clicked();
        }

        contentItem: ColumnLayout {

            // Title and subtitle
            ColumnLayout {
                id: headerLayout

                Layout.fillWidth: true
                Layout.minimumHeight: Kirigami.Units.gridUnit * 2
                spacing: 0

                Kirigami.Heading {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumWidth: thumbnailArea.implicitWidth // HACK
                    horizontalAlignment: Text.AlignHCenter
                    level: 3
                    text: delegate.text
                    maximumLineCount: caption.visible ? 1 : 2
                    wrapMode: caption.visible ? Text.NoWrap : Text.Wrap
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
            }

            // Thumbnail
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

            // Contextual buttons on the bottom
            Kirigami.ActionToolBar {
                Layout.fillWidth: true
                implicitHeight: Kirigami.Units.gridUnit * 2
                actions: delegate.actions
            }
        }
    }

    QQC2.ToolTip.delay: 1000
    QQC2.ToolTip.timeout: 5000
    QQC2.ToolTip.visible: hovered && delegate.toolTip.length > 0
    QQC2.ToolTip.text: toolTip
}
