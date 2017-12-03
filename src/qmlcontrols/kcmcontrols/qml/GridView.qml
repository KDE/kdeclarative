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
import org.kde.kirigami 2.3 as Kirigami

/**
 * A ScrollView containing a GridView, with the default behavior about
 * sizing and background as recomended by the user interface guidelines
 * For most KControl modules, it's recomended to use instead the GridViewKCM
 * component as the root element of your module.
 * @see GridViewKCM
 */
QtControls.ScrollView {
    id: scroll

    /**
     * view: GridView
     * Exposes the internal GridView: in order to set a model or a delegate to it,
     * use the following code:
     * @code
     * import org.kde.kcm 1.1 as KCM
     * KCM.GridView {
     *     view.model: kcm.model
     *     view.delegate: KCM.GridDelegate {...}
     * }
     * @endcode
     */
    property alias view: view


    implicitWidth: Math.max(view.cellWidth * 2 + internal.scrollBarSpace
                , Math.floor(internal.availableWidth / view.cellWidth) * view.cellWidth + internal.scrollBarSpace + 4)
    activeFocusOnTab: false
    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    Component.onCompleted: scroll.background.visible = true;

    GridView {
        id: view
        property int implicitCellWidth: Kirigami.Units.gridUnit * 10
        property int implicitCellHeight: Math.round(implicitCellWidth / 1.6) + Kirigami.Units.gridUnit
        QtObject {
            id: internal
            readonly property int availableWidth: scroll.parent.width - internal.scrollBarSpace - 4
            readonly property int scrollBarSpace: scroll.QtControls.ScrollBar.vertical.width
        }
        anchors {
            fill: parent
            margins: 2
            leftMargin: contentHeight <= height ? internal.scrollBarSpace/2 : 2
            rightMargin: contentHeight <= height ? internal.scrollBarSpace/2 : internal.scrollBarSpace + 2
        }
        clip: true
        activeFocusOnTab: true

        cellWidth: Math.min(Math.floor(internal.availableWidth / 2), implicitCellWidth)
        cellHeight: Math.round(cellWidth / 1.6) + Kirigami.Units.gridUnit

        keyNavigationEnabled: true
        keyNavigationWraps: true
        highlightMoveDuration: 0
    }
    QtControls.ScrollBar.horizontal.visible: false
}
