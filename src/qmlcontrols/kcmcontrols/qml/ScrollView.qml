/*
    SPDX-FileCopyrightText: 2018 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QtControls
import org.kde.kirigami 2.3 as Kirigami

/**
 * A ScrollView containing a GridView, with the default behavior about
 * sizing and background as recommended by the user interface guidelines
 * For most KControl modules, it's recommended to use instead the GridViewKCM
 * component as the root element of your module.
 * @code
 * import org.kde.kcm 1.2 as KCM
 * KCM.ScrollView {
 *     view: ListView {
 *       ...
 *     }
 * }
 * @endcode
 * @see GridViewKCM
 */
QtControls.ScrollView {
    id: scroll

    /**
     * view: GridView
     * Exposes the internal flickable
     */
    property Flickable view
    contentItem: view
    onViewChanged: {
        view.parent = scroll;
    }

    activeFocusOnTab: false
    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    Component.onCompleted: scroll.background.visible = true;

    
    QtControls.ScrollBar.horizontal.visible: false
}
