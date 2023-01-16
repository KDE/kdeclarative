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

    readonly property int margins: 6 // Layout_ChildMarginWidth from Breeze

    /**
     * extraFooterTopPadding: bool
     * Whether to add extra top padding to an empty footer.  Use the default
     * value of true for KCMs in System Settings, because otherwise the Apply,
     * Help, and Defaults buttons provided by System Settings won't have enough
     * top padding and the button bar will look ugly.
     * When using this component outside of System Settings where there is no
     * such restriction, or in System Settings KCMs that don't show Apply, Help,
     * or Defaults buttons, set it to false.
     * Default: true
     *
     * @since 5.101
     */
    property bool extraFooterTopPadding: true

    property bool __flickableOverflows: flickable.contentHeight + flickable.topMargin + flickable.bottomMargin > flickable.height

    // Context properties are not reliable
    title: (typeof kcm !== "undefined") ? kcm.name : ""

    // Make pages fill the whole view by default
    Kirigami.ColumnView.fillWidth: true

    leftPadding: root.margins
    topPadding:  root.margins
    rightPadding: root.margins
    bottomPadding: root.margins

    header: QtControls.Control {
        id: headerParent

        readonly property bool contentVisible: contentItem && contentItem.visible && contentItem.implicitHeight

        height: contentVisible ? implicitHeight : 0
        leftPadding: 0
        topPadding: 0
        rightPadding: 0
        bottomPadding: 0
        // TODO KF6: uncomment these lines. We didn't do it in KF5 times because
        // it would have regressed CommandOutputKCM in KInfoCenter, which was
        // created before this padding was added, and added its own. If we add
        // padding in the base KCM component, CommandOutputKCM will have double
        // padding and look bad.
        // leftPadding: root.margins
        // topPadding: root.margins
        // rightPadding: root.margins
        // bottomPadding: root.margins

        // When the page is scrollable, we need to add a line below the header
        // ourselves to separate it from the view
        Kirigami.Separator {
            z: 999
            anchors {
                left: parent.left
                right: parent.right
                top: parent.bottom
            }
            visible: headerParent.contentVisible || (__flickableOverflows && !Kirigami.Settings.isMobile)
        }
    }

    footer: QtControls.Control {
        id: footerParent

        readonly property bool contentVisible: contentItem && contentItem.visible && contentItem.implicitHeight

        height: contentVisible ? implicitHeight : (root.extraFooterTopPadding ? root.margins : 0)
        leftPadding: 0
        topPadding: 0
        rightPadding: 0
        bottomPadding: 0
        // TODO KF6: uncomment these lines. We didn't do it in KF5 times because
        // it would have regressed CommandOutputKCM in KInfoCenter, which was
        // created before this padding was added, and added its own. If we add
        // padding in the base KCM component, CommandOutputKCM will have double
        // padding and look bad.
        // created before this padding was added and added its own.
        // leftPadding: root.margins
        // topPadding: root.margins
        // rightPadding: root.margins
        // bottomPadding: root.margins

        // When the page is scrollable, we need to add a line above the footer
        // ourselves to separate it from the view
        Kirigami.Separator {
            z: 999
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.top
            }
            visible:  footerParent.contentVisible || (root.__flickableOverflows && !Kirigami.Settings.isMobile)
        }
    }

    Component.onCompleted: {
        if (footer && footer !== footerParent) {
            const f = footer

            footerParent.contentItem = f
            footer = footerParent
            footer.visible = true
            f.parent = footerParent
        }

        if (header && header !== headerParent) {
            const h = header

            // Revert the effect of repeated onHeaderChanged invocations
            // during initialization in Page super-type.
            h.anchors.top = undefined

            headerParent.contentItem = h
            header = headerParent
            header.visible = true
            h.parent = headerParent
        }

        //Search overlaysheets in contentItem, parent to root if found
        for (const i in contentItem.data) {
            const child = contentItem.data[i];
            if (child instanceof Kirigami.OverlaySheet) {
                if (!child.parent) {
                    child.parent = root;
                }
                root.data.push(child);
            }
        }
    }
}
