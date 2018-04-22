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

    /**
     * An header element to be put on top of the View,
     * The difference with the KCM header is that this item will have te same width as the view, whild the KCM top header will span for the whole KCM size
     */
    property Item viewHeader
    property Item viewFooter

    title: kcm.name
    implicitWidth: Kirigami.Units.gridUnit * 30
    implicitHeight: Kirigami.Units.gridUnit * 40
    flickable: scroll.view

    topPadding: 0
    leftPadding: 0
    rightPadding: 0
    bottomPadding: footer ? Kirigami.Units.smallSpacing : 0

    Component.onCompleted: {
        if (footer && Kirigami.Settings.isMobile) {
            footer.anchors.leftMargin = Kirigami.Units.smallSpacing
            footer.anchors.rightMargin = Kirigami.Units.smallSpacing
            footer.anchors.bottomMargin = Kirigami.Units.smallSpacing
            footer.anchors.left = root.left;
            footer.anchors.right = root.right;
            footer.anchors.bottom = root.bottom;
        }
    }
    KCM.ScrollView {
        id: scroll
        anchors.fill: parent
    }
}
