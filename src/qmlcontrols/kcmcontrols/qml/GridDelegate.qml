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
Kirigami.Card {
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

    implicitWidth: GridView.view.cellWidth - Kirigami.Units.gridUnit * 2
    implicitHeight: GridView.view.cellHeight - Kirigami.Units.gridUnit * 2

    // FIXME: center the card inside the grid cell

    showClickFeedback: true
    highlighted: delegate.GridView.isCurrentItem

    // Title and optional subtitle
    header: ColumnLayout {
        // FIXME: seems to be ignored? Header layout is too tall
        implicitHeight: Kirigami.Units.gridUnit * 2

        Kirigami.Heading {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: caption.visible? Text.AlignBottom : Text.AlignVCenter
            level: 3
            text: delegate.text
            elide: Text.ElideRight
            font.bold: delegate.GridView.isCurrentItem
        }
        QQC2.Label {
            id: caption
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
            visible: delegate.subtitle.length > 0
            opacity: 0.6
            text: delegate.subtitle
            font.pointSize: theme.smallestFont.pointSize
            font.bold: delegate.GridView.isCurrentItem
            elide: Text.ElideRight
        }
    }

    // Thumbnail
    contentItem: ColumnLayout {
        spacing: 0

        // FIXME: this whole thing should vertically maximize itself
        // Layout.fillHeight: true // doesn't work
        // anchors.fill: parent // doesn't work

        // FIXME: this content item un-clips itself and gets laid out strangely when the window is resized
        Rectangle {
            id: thumbnailArea
            radius: Kirigami.Units.smallSpacing/2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: Kirigami.Units.gridUnit * 4
            Layout.margins: Kirigami.Units.smallSpacing

            clip: true

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
            // FIXME: this doesn't succeed in adding spacing below the separator
            Layout.bottomMargin: Kirigami.Units.smallSpacing
        }
    }

    QQC2.ToolTip.delay: 1000
    QQC2.ToolTip.timeout: 5000
    QQC2.ToolTip.visible: hovered && delegate.toolTip.length > 0
    QQC2.ToolTip.text: toolTip
}
