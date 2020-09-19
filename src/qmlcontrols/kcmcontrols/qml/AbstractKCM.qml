/*
    SPDX-FileCopyrightText: 2020 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.7
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.2 as Kirigami
import org.kde.kcm 1.1 as KCM

/**
 * This component is intended to be used as root item for
 * KCMs with arbitrary content. Unlike SimpleKCM this does NOT
 * provide a scrollable view, The developer will have to manage
 * their own scrollviews.
 * Most of the times SimpleKCM should be used instead
 * @code
 * import org.kde.kcm 1.1 as KCM
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

    leftPadding: Kirigami.Settings.isMobile ? 0 : 4
    topPadding: headerParent.contentItem ? 0 : (Kirigami.Settings.isMobile ? 0 : 4)
    rightPadding: (Kirigami.Settings.isMobile ? 0 : 4)
    bottomPadding: footerParent.contentItem ? 0 : (Kirigami.Settings.isMobile ? 0 : 4)

    header: QtControls.Control {
        id: headerParent
        visible: contentItem ? contentItem.visible : false
        height: visible ? implicitHeight : 0
        leftPadding: 4
        topPadding: 4
        rightPadding: 4
        bottomPadding: 4
    }

    footer: QtControls.Control {
        id: footerParent
        visible: contentItem ? contentItem.visible : false
        height: visible ? implicitHeight : 0
        leftPadding: 4
        topPadding: 4
        rightPadding: 4
        bottomPadding: 4
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
    }
}
