/*
 * Copyright (C) 2015 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

import QtQuick 2.3
import QtQuick.Controls 1.3

import org.kde.private.kquickcontrols 2.0 as Private

/**
 * A text area that provides a context menu similar to a traditional widget
 * @inherit QtQuick.Controls.TextArea
 */
TextArea {
    id: textArea

    menu: Private.TextMenu {
        target: textArea
    }
}

