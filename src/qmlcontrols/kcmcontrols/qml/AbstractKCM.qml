/*
    SPDX-FileCopyrightText: 2020 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.7
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.14 as Kirigami
import org.kde.kcm 1.1 as KCM

/**
 * This component is intended to be used as root item for
 * KCMs with arbitrary content. Unlike SimpleKCM this does NOT
 * provide a scrollable view, The developer will have to manage
 * their own scrollviews.
 * Most of the times SimpleKCM should be used instead
 * @code
 * import org.kde.kcm 1.4 as KCM
 * import org.kde.kirigami 2.3 as Kirigami
 * KCM.AbstractKCM {
 *     RowLayout {
 *         QQC2.ScrollView {
 *             ...
 *         }
 *         QQC2.ScrollView {
 *             ...
 *         }
 *     }
 *     footer: Item {...}
 * }
 * @endcode
 * @inherits org.kde.kirigami.Page
 */
Kirigami.Page {
    id: root

    readonly property int margins: 6 // Layout_ChildMarginWidth from Breeze

    /**
     * framedView: bool
     * Whether to use this component as the base of a "framed" KCM with an
     * inner scrollview that draws its own frame.
     * Default: true
     *
     * @since 5.90
     */
    property bool framedView: true

    title: kcm.name

    // Make pages fill the whole view by default
    Kirigami.ColumnView.fillWidth: true

    leftPadding: root.framedView ? root.margins : 0
    topPadding: headerParent.contentVisible || !root.framedView ? 0 : root.margins
    rightPadding: root.framedView ? root.margins : 0
    bottomPadding: footerParent.contentVisible || !root.framedView ? 0 : root.margins

    header: QtControls.Control {
        id: headerParent
        readonly property bool contentVisible: contentItem && contentItem.visible && contentItem.implicitHeight
        height: contentVisible ? implicitHeight : 0
        leftPadding: root.margins
        topPadding: root.margins
        rightPadding: root.margins
        bottomPadding: root.margins

        // When the scrollview isn't drawing its own frame, we need to add a
        // line below the header (when visible) to separate it from the view
        Kirigami.Separator {
            z: 999
            anchors {
                left: parent.left
                right: parent.right
                top: parent.bottom
            }
            visible: !root.framedView && headerParent.contentVisible
        }
    }

    // View background, shown when the scrollview isn't drawing its own frame
    Rectangle {
        anchors.fill: parent
        visible: !root.framedView
        Kirigami.Theme.colorSet: Kirigami.Theme.View
        Kirigami.Theme.inherit: false
        color: Kirigami.Theme.backgroundColor
    }

    footer: QtControls.Control {
        id: footerParent
        readonly property bool contentVisible: contentItem && contentItem.visible && contentItem.implicitHeight

        // When the scrollview isn't drawing its own frame, we need to add
        // extra padding on top when there is no footer content, or else the
        // Apply, Help, and Defaults buttons provided by System Settings won't
        // have enough top padding. This isn't a problem when the inner scrollview
        // draws its own frame because its own outer margins provide this
        height: contentVisible ? implicitHeight : (root.framedView ? 0 : root.margins)
        leftPadding: root.margins
        topPadding: root.margins
        rightPadding: root.margins
        bottomPadding: root.margins

        // When the scrollview isn't drawing its own frame, we need to add a
        // line above the footer ourselves to separate it from the view
        Kirigami.Separator {
            z: 999
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.top
            }
            // Deliberately not checking for footerParent.contentVisible because
            // we always want the footer line to be visible when the scrollview
            // doesn't have a frame of its own, because System Settings always
            // adds its own footer for the Apply, Help, and Defaults buttons
            visible: !root.framedView
        }
    }

    Component.onCompleted: {
        if (footer && footer != footerParent) {
            var f = footer

            footerParent.contentItem = f
            footer = footerParent
            footer.visible = true
            f.parent = footerParent
        }

        if (header && header != headerParent) {
            var h = header

            headerParent.contentItem = h
            header = headerParent
            header.visible = true
            h.parent = headerParent
        }

        //Search overlaysheets in contentItem, parent to root if found
        for (let i in contentItem.data) {
            let child = contentItem.data[i];
            if (child instanceof Kirigami.OverlaySheet) {
                if (!child.parent) {
                    child.parent = root;
                }
                root.data.push(child);
            }
        }
    }
}
