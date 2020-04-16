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
import QtQuick.Controls 2.12
import org.kde.kcm 1.3 as KCM
import org.kde.kirigami 2.2 as Kirigami

/**
 * A simple icon which automatically picks an icon, displays or hides itself
 * depending on a setting state.
 *
 * @since 5.70
 */
ToolButton {
    id: root

    /**
     * settingState: KCM.SettingStateProxy
     * The state used by the indicator to decides if and how it is displayed
     */
    property KCM.SettingStateProxy settingState

    visible: settingState && !settingState.defaulted
    focusPolicy: Qt.NoFocus

    implicitWidth: icon.width + Kirigami.Units.largeSpacing
    implicitHeight: icon.height + Kirigami.Units.largeSpacing

    icon.name: "edit-reset"
    icon.width: Kirigami.Units.iconSizes.small
    icon.height: Kirigami.Units.iconSizes.small

    onClicked: settingState.resetToDefault()
}
