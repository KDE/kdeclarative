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
 * KControl modules which are based upon a grid view of thumbnails, such as theme
 * or wallpaper selectors.
 * It has a big GridView as its main item, the implementation is free to add extra
 * content in the header or footer properties.
 * @code
 * import org.kde.kcm 1.1 as KCM
 * KCM.GridViewKCM {
 *     header: Item {...}
 *     view.model: kcm.model
 *     view.delegate: KCM.GridDelegate {...}
 *     footer: Item {...}
 * }
 * @endcode
 * @inherits org.kde.kirigami.Page
 */
Kirigami.Page {
    id: root

    /**
     * view: GridView
     * Exposes the internal GridView: in order to set a model or a delegate to it,
     * use the following code:
     * @code
     * import org.kde.kcm 1.1 as KCM
     * KCM.GridViewKCM {
     *     ...
     *     view.model: kcm.model
     *     view.delegate: KCM.GridDelegate {...}
     *     ...
     * }
     * @endcode
     */
    property alias view: scroll.view

    title: kcm.name
    implicitWidth: view.implicitCellWidth * 3 + scroll.QtControls.ScrollBar.vertical.width + Kirigami.Units.gridUnit
    implicitHeight: view.implicitCellHeight * 3 + (header ? header.height : 0) + (footer ? footer.height : 0) + Kirigami.Units.gridUnit

    topPadding: 0
    leftPadding: 0
    rightPadding: 0
    bottomPadding: footer ? Kirigami.Units.smallSpacing : 0

    KCM.GridView {
        id: scroll
        width: Math.min(implicitWidth, root.width)
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }
}
