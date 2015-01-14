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
import QtQuick.Controls 1.1

import org.kde.private.kquickcontrols 2.0 as Private

Menu {
    property Item target

    Private.TranslationContext {
        id: _tr
        domain: "kdeclarative5"
    }

    Action {
        id: undoAction
        shortcut: StandardKey.Undo
        iconName: "edit-undo"
        text: _tr.i18n("Undo")
        enabled: target.canUndo
        onTriggered: target.undo()
    }
    Action {
        id: redoAction
        shortcut: StandardKey.Redo
        iconName: "edit-redo"
        text: _tr.i18n("Redo")
        enabled: target.canRedo
        onTriggered: target.redo()
    }

    Action {
        id: cutAction
        shortcut: StandardKey.Cut
        iconName: "edit-cut"
        text: _tr.i18n("Cut")
        enabled: !target.readOnly && target.selectedText.length > 0
        onTriggered: target.cut()
    }
    Action {
        id: copyAction
        shortcut: StandardKey.Copy
        iconName: "edit-copy"
        text: _tr.i18n("Copy")
        enabled: target.selectedText.length > 0
        onTriggered: target.copy()
    }
     Action {
         id: pasteAction
         shortcut: StandardKey.Paste
         iconName: "edit-paste"
         text: _tr.i18n("Paste")
         enabled: target.canPaste
         onTriggered: target.paste()
     }
     Action {
        id: deleteAction
        shortcut: StandardKey.Delete
        iconName: "edit-delete"
        text: _tr.i18n("Delete")
        enabled: !target.readOnly && target.selectedText.length > 0
        onTriggered: {
            var before = target.text.substring(0, target.selectionStart)
            var after = target.text.substring(target.selectionEnd, target.text.length)
            var position = target.selectionStart
            target.text = before + after
            target.cursorPosition = position // setting text moves the cursor to the end
        }
    }
    Action {
        id: clearAction
        shortcut: StandardKey.DeleteCompleteLine
        text: _tr.i18n("Clear")
        iconName: "edit-clear"
        enabled: !target.readOnly && target.text.length > 0
        onTriggered: target.text = ""
    }

    Action {
        id: selectAllAction
        shortcut: StandardKey.SelectAll
        text: _tr.i18n("Select All")
        enabled: target.selectionStart > 0 || target.selectionEnd < target.text.length
        onTriggered: target.selectAll()
    }

    MenuItem { action: undoAction }
    MenuItem { action: redoAction }

    MenuSeparator {}

    MenuItem { action: cutAction }
    MenuItem { action: copyAction }
    MenuItem { action: pasteAction }
    MenuItem { action: deleteAction }
    MenuItem { action: clearAction }

    MenuSeparator { }

    MenuItem { action: selectAllAction }

}
