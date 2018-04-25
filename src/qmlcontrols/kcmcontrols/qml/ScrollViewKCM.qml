/*
   Copyright (c) 2017 Marco Martin <mart@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.2 as Kirigami
import org.kde.kcm 1.1 as KCM
import "." as Priv

/**
 * This component is intended to be used as the root item for most of the
 * KControl modules which are based upon a list view or another vertical flickable
 * It has a big GridView as its main item, the implementation is free to add extra
 * content in the header or footer properties.
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
 * @inherits org.kde.kirigami.Page
 */
Kirigami.Page {
    id: root

    /**
     * view: GridView
     * Exposes the internal flickable
     */
    property alias view: scroll.view

    title: kcm.name
    //implicitWidth: Kirigami.Units.gridUnit * 30
    //implicitHeight: Kirigami.Units.gridUnit * 40
    //flickable: scroll.view

    //NOTE: this should be smallspacing buit we need a pixel size in order to align with systemsettings widgets
    leftPadding: Kirigami.Settings.isMobile ? 0 : 4
    topPadding: headerParent.contentItem ? 0 : leftPadding
    rightPadding: leftPadding
    bottomPadding: footerParent.contentItem ? 0 : leftPadding

    header: QtControls.Control {
        id: headerParent
        padding: 4
    }

    footer: QtControls.Control {
        id: footerParent
        padding: 4
    }
    Component.onCompleted: {
        if (footer && footer != footerParent) {
            var f = footer

            footerParent.contentItem = f
            footer = footerParent
            f.visible = true
            f.parent = footerParent
        }

        if (header && header != headerParent) {
            var f = header

            headerParent.contentItem = f
            header = headerParent
            f.visible = true
            f.parent = headerParent
        }
    }
    Priv.ScrollView {
        id: scroll
        anchors.fill: parent
    }
}
