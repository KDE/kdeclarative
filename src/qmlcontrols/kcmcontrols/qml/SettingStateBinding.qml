/*
   Copyright (c) 2020 Kevin Ottens <kevin.ottens@enioka.com>

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

import QtQuick 2.8
import QtQuick.Layouts 1.3
import org.kde.kcm 1.3 as KCM
import org.kde.kcm.private 1.3

/**
 * SettingStateBinding automatically impacts the representation
 * of an item based on the state of a setting. It will disable
 * the item if the setting is immutable and use a visual indicator
 * for the state of the setting.
 *
 * This is a higher level convenience wrapper for KCM.SettingStateProxy
 * and KCM.SettingStateIndicator.
 *
 * @since 5.70
 */
Item {
    id: root

    /**
     * target: Item
     * The graphical element whose state we want to manage based on a setting
     */
    property Item target

    /**
     * configObject: KCoreConfigSkeleton
     * The config object which will be monitored for setting state changes
     */
    property alias configObject: settingState.configObject

    /**
     * itemName: string
     * The name of the item representing the setting in the config object
     */
    property alias itemName: settingState.itemName

    /**
     * extraEnabledPredicate: bool
     * SettingStateBinding will manage the enabled property of the target
     * based on the immutability state of the setting it represents. But,
     * sometimes that enabled state needs to bind to other properties as
     * well to be computed properly. This extra predicate will thus be
     * combined with the immutability state of the setting to determine
     * the effective enabled state of the target.
     */
    property bool extraEnabledPredicate: true

    /**
     * indicatorAsOverlay: bool
     * If true the state indicator will be displayed inside the boundaries
     * of the target item which is convenient for items with lots of empty
     * space like list or grid views (default is false)
     */
    property bool indicatorAsOverlay: false


    QtObject {
        id: impl
        property KCM.SettingStateIndicator indicator: null
    }

    onTargetChanged: {
        if (impl.indicator) {
            impl.indicator.destroy()
            impl.indicator = null
        }

        if (target) {
            impl.indicator = indicatorComponent.createObject(target, {
                settingState: settingState
            })
        }
    }

    // We use it via a Component because we potentially need
    // to escape the parent/siblings only constraint for anchoring
    // (since we anchor to root.target which could be anywhere)
    Component {
        id: indicatorComponent
        KCM.SettingStateIndicator {
            readonly property real directionFactor: (Qt.application.layoutDirection === Qt.LeftToRight ? 1 : -1)
            x: helper.leftCoord - (root.indicatorAsOverlay ? width : 0) * directionFactor
            y: root.indicatorAsOverlay ? 0 : Math.round((parent.height - height) / 2)
            settingState: impl.settingState
        }
    }

    Binding {
        target: root.target
        property: "enabled"
        value: extraEnabledPredicate && !settingState.immutable
    }

    KCM.SettingStateProxy {
        id: settingState
    }

    SettingStateBindingPrivate {
        id: helper
        target: root.target
        indicator: impl.indicator
        indicatorAsOverlay: root.indicatorAsOverlay
    }
}
