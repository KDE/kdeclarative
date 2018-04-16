/*
 * Copyright (C) 2018 David Edmundson <davidedmundson@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

import QtQuick 2.6
import QtQuick.Controls 2.0

import org.kde.private.kquickcontrols 2.0 as KQuickControlsPrivate
import org.kde.kconfig 1.0 // for KAuthorized

Button
{
    id: button
    property alias ksnrcFile: _helper.ksnrcFile
    property alias dialogTitle: _helper.title
    /**
     * Emitted when a user finishes installing new items
     */
    signals: entriesChanged

    text: _tr.i18n("Get New...")
    icon.name: "get-hot-new-stuff" //DAVE - either needs up to date Qt or I use QQC1 here. QQC1.Button has a palette bug

    enabled: KAuthorized.authorize("ghns")

    KQuickControlsPrivate.TranslationContext {
        id: _tr
        domain: "kdeclarative5"
    }

    KQuickControlsPrivate.GHNSHelper {
        id: _helper
        onEntriesChanged: button.entriesChanged
    }

    onClicked: _helper.getNew(button)
}
