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
import QtQml 2.14
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
 * @since 5.73
 */
Item {
    id: root

    /**
     * target: Item
     * The graphical element whose state we want to manage based on a setting
     * If target is not set, it will try to find the visual parent item
     */
    property alias target: helper.target

    /**
     * configObject: KCoreConfigSkeleton
     * The config object which will be monitored for setting state changes
     */
    property alias configObject: settingState.configObject

    /**
     * settingName: string
     * The name of the setting in the config object to be monitored
     */
    property alias settingName: settingState.settingName

    /**
     * extraEnabledConditions: bool
     * SettingStateBinding will manage the enabled property of the target
     * based on the immutability state of the setting it represents. But,
     * sometimes that enabled state needs to bind to other properties as
     * well to be computed properly. This extra condition will thus be
     * combined with the immutability state of the setting to determine
     * the effective enabled state of the target.
     */
    property bool extraEnabledConditions: true

    Binding {
        when: helper.target
        target: helper.target
        property: "enabled"
        value:  extraEnabledConditions && !settingState.immutable
        restoreMode: Binding.RestoreBinding
    }

    Binding {
        when: helper.target
        target: helper
        property: "highlight"
        value: !settingState.defaulted
        restoreMode: Binding.RestoreBinding
    }

    KCM.SettingStateProxy {
        id: settingState
    }

    SettingStateBindingPrivate {
        id: helper
        defaultIndicatorVisible: kcm.defaultsIndicatorsVisible
    }

    Component.onCompleted: {
        helper.updateTarget();
    }
}
