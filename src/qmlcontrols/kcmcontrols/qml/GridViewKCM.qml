/*
    SPDX-FileCopyrightText: 2017 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.2 as Kirigami
import org.kde.kcm 1.1 as KCM

/**
 * This component is intended to be used as the root item for KCMs that are based upon
 * a grid view of thumbnails, such as the theme or wallpaper selectors.
 * It contains a GridView as its main item.
 * It is possible to specify a header and footer component.
 * @code
 * import org.kde.kcm 1.1 as KCM
 * KCM.GridViewKCM {
 *     header: Item {...}
 *     view.model: kcm.model
 *     view.delegate: KCM.GridDelegate {...}
 *     footer: Item {...}
 * }
 * @endcode
 */
AbstractKCM {
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

    implicitWidth: {
        var width = 0;

        // Show three columns at once, every colum occupies implicitCellWidth + Units.gridUnit
        width += 3 * (view.implicitCellWidth + Kirigami.Units.gridUnit);

        var scrollBar = scroll.QtControls.ScrollBar.vertical;
        width += scrollBar.width + scrollBar.leftPadding + scrollBar.rightPadding;

        width += scroll.leftPadding + scroll.rightPadding
        width += root.leftPadding + root.rightPadding;

        return width;
    }
    implicitHeight: view.implicitCellHeight * 3 + (header ? header.height : 0) + (footer ? footer.height : 0) + Kirigami.Units.gridUnit

    flickable: scroll.view

    KCM.GridView {
        id: scroll
        anchors.fill: parent
    }
}
