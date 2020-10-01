/*
    SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
    SPDX-FileCopyrightText: 2020 David Redondo <kde@david.redondo.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.8
import org.kde.kcm 1.3 as KCM
import org.kde.kcm.private 1.3

/**
 * SettingHighlighter automatically impacts the representation of an item based on
 * the value of a setting. When you are using this item you need to manually
 * manage whether the highlighting is enabled or not. For a higher level component
 * see KCM.SettingStateBinding which will manage the state of the Item
 * @since 5.75
 */
Item {
    id: root

    /**
     * target: Item
     * The graphical element whose appearance will be altered.
     * If target is not set, it will try to find the visual parent item
     */
    property alias target: helper.target

    /***
     * highlight: bool
     * Whether the target will be highlighted.
     */
    property alias highlight: helper.highlight

    SettingHighlighterPrivate {
        id: helper
        defaultIndicatorVisible: kcm.defaultsIndicatorsVisible
    }

    Component.onCompleted: {
        helper.updateTarget();
    }
}

