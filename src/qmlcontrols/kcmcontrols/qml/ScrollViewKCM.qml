/*
    SPDX-FileCopyrightText: 2017 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.2 as Kirigami
import org.kde.kcm 1.1 as KCM
import "." as Priv

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

    Priv.ScrollView {
        id: scroll
        anchors.fill: parent
    }
}
