/*
    SPDX-FileCopyrightText: 2017 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.7
import org.kde.kcm 1.1 as KCM

/**
 * This component is intended to be used as the root item for KCMs that are based upon a list view or another vertical flickable.
 * It contains a ScrollView as its main item.
 * It is possible to specify a header and footer component.
 * @code
 * import org.kde.kcm 1.2 as KCM
 * KCM.ScrollViewKCM {
 *     header: Item {...}
 *     view: ListView {
 *       ...
 *     }
 *     footer: Item {...}
 * }
 * @endcode
 */
AbstractKCM {
    id: root

    /**
     * view: ScrollView
     * Exposes the internal flickable
     */
    property alias view: scroll.view

    /**
     * framedView: bool
     * Whether to draw a frame around the KCM's inner scrollable list view.
     * Default: false
     *
     * @since 5.90
     */
    framedView: false

    Priv.ScrollView {
        id: scroll
        anchors.fill: parent
        framedView: root.framedView
    }
}
