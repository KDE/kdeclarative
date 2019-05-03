import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import org.kde.private.kquickcontrols 2.0 as KQuickControlsPrivate

RowLayout {
    property alias showClearButton: clearButton.visible
    property alias modifierlessAllowed: _helper.modifierlessAllowed
    property alias multiKeyShortcutsAllowed: _helper.multiKeyShortcutsAllowed
    property alias keySequence: _helper.keySequence


    KQuickControlsPrivate.KeySequenceHelper {
        id: _helper

        onCaptureFinished: {
            mainButton.checked = false;
        }
    }

    KQuickControlsPrivate.TranslationContext {
        id: _tr
        domain: "kdeclarative5"
    }

    Button
    {
        id: mainButton

        iconName: "configure"

        property string shortcut
        checkable: true
        focus: checked

        text: _helper.shortcutDisplay
        tooltip: _tr.i18n("Click on the button, then enter the shortcut like you would in the program.\nExample for Ctrl+A: hold the Ctrl key and press A.")

        onCheckedChanged: {
            if (checked) {
                mainButton.forceActiveFocus()
                _helper.captureKeySequence()
            }
        }

        onFocusChanged: {
            if (!focus) {
                mainButton.checked = false
            }
        }

        Keys.onPressed: {
            _helper.keyPressed(event.key, event.modifiers);
            event.accepted = true;
        }
        Keys.onReleased: {
            _helper.keyReleased(event.key, event.modifiers);
            event.accepted = true;
        }
    }

    Button {
        id: clearButton
        Layout.fillHeight: true
        Layout.preferredWidth: height
        onClicked: _helper.clearKeySequence();

        //icon name determines the direction of the arrow, NOT the direction of the app layout
        iconName: Qt.application.layoutDirection == Qt.LeftToRight ? "edit-clear-locationbar-rtl" : "edit-clear-locationbar-ltr"
    }
}
