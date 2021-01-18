/*
    SPDX-FileCopyrightText: 2017 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.7
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.14 as Kirigami
import org.kde.kcm 1.1 as KCM

/**
 * This component is intended to be used as root item for
 * KCMs with arbitrary content. Often a Kirigami.FormLayout
 * is used as main element.
 * It is possible to specify a header and footer component.
 * @code
 * import org.kde.kcm 1.1 as KCM
 * import org.kde.kirigami 2.3 as Kirigami
 * KCM.SimpleKCM {
 *     Kirigami.FormLayout {
 *        TextField {
 *           Kirigami.FormData.label: "Label:"
 *        }
 *        TextField {
 *           Kirigami.FormData.label: "Label:"
 *        }
 *     }
 *     footer: Item {...}
 * }
 * @endcode
 * @inherits org.kde.kirigami.ScrollablePage
 */
Kirigami.ScrollablePage {
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
    }

    footer: QtControls.Control {
        id: footerParent
        height: contentItem ? implicitHeight : 0
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

    children: [
        Kirigami.Separator {
            z: 999
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                topMargin: root.header.visible ? root.header.height : 0
            }
            visible: !root.flickable.atYBeginning && !Kirigami.Settings.isMobile
        },
        Kirigami.Separator {
            z: 999
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                bottomMargin: root.footer.visible ? root.footer.height : 0
            }
            visible: !root.flickable.atYEnd && !Kirigami.Settings.isMobile
        }
    ]
}
