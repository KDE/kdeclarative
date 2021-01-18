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

    title: kcm.name

    // Make pages fill the whole view by default
    Kirigami.ColumnView.fillWidth: true

    leftPadding: 6 // Layout_ChildMarginWidth from Breeze
    topPadding: headerParent.contentItem ? 0 : leftPadding
    rightPadding: leftPadding
    bottomPadding: footerParent.contentItem ? 0 : leftPadding

    header: QtControls.Control {
        id: headerParent
        height: contentItem ? implicitHeight : 0
        leftPadding: 6 // Layout_ChildMarginWidth from Breeze
        topPadding: leftPadding
        rightPadding: leftPadding
        bottomPadding: leftPadding
    }

    footer: QtControls.Control {
        id: footerParent
        height: contentItem ? implicitHeight : 0
        leftPadding: 6 // Layout_ChildMarginWidth from Breeze
        topPadding: leftPadding
        rightPadding: leftPadding
        bottomPadding: leftPadding
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
