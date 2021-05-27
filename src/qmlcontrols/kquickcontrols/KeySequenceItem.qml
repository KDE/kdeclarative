import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.1
import QtQuick.Window 2.15

import org.kde.private.kquickcontrols 2.0 as KQuickControlsPrivate

RowLayout {
    id: root
    property alias showClearButton: clearButton.visible
    property alias modifierlessAllowed: _helper.modifierlessAllowed
    property alias multiKeyShortcutsAllowed: _helper.multiKeyShortcutsAllowed
    property var keySequence

    /**
     * This property controls which types of shortcuts are checked for conflicts when the keySequence
     * is set. If a conflict is detected, a messagebox will be shown asking the user to confirm their
     * input. Valid values are combinations of the following flags:
     *   - @p ShortcutType.None Do not check for conflicts.
     *   - @p ShortcutType.StandardShortcuts Check against standard shortcuts. @see KStandardshortcut
     *   - @p ShortcutType.GlobalShortcuts Check against global shortcuts. @see KGlobalAccel
     *
     * The default is `ShortcutType.GlobalShortcuts | ShortcutType.StandardShortcut`
     * @since 5.74
     */
    property alias checkForConflictsAgainst: _helper.checkAgainstShortcutTypes

    /**
     * This signal is emitted after the user introduces a new key sequence
     *
     * @since 5.68
     */
    signal captureFinished()

    /**
     * Start capturing a key sequence. This equivalent to the user clicking on the main button of the item
     * @since 5.70
     */
    function startCapturing() {
        mainButton.checked = true
    }

    KQuickControlsPrivate.KeySequenceHelper {
        id: _helper
        window: renderWindow(parent.Window.window)
        onGotKeySequence: {
            if (isKeySequenceAvailable(keySequence)) {
                root.keySequence = keySequence
            }
            mainButton.checked = false;
            root.captureFinished();
        }
    }

    KQuickControlsPrivate.TranslationContext {
        id: _tr
        domain: "kdeclarative5"
    }

    Button
    {
        id: mainButton

        icon.name: "configure"

        checkable: true
        focus: checked

        hoverEnabled: true

        text: {
            const keys = _helper.isRecording ? _helper.currentKeySequence : parent.keySequence
            let text = " " // This space is intentional
            if (keys == undefined || _helper.keySequenceIsEmpty(keys)) {
                if (_helper.isRecording) {
                    text += _tr.i18nc("What the user inputs now will be taken as the new shortcut", "Input")
                } else {
                     text += _tr.i18nc("No shortcut defined", "None");
                }
            } else {
                // Single ampersand gets interpreted by the button as a mnemonic
                // and removed; replace it with a double ampersand so that it
                // will be displayed by the button as a single ampersand, or
                // else shortcuts with the actual ampersand character will
                // appear to be partially empty.
                text += _helper.keySequenceNativeText(keys).replace('&', '&&')
            }
            return _helper.isRecording ? text.concat(" ... ") : text.concat(" ")
        }

        ToolTip.visible: hovered
        ToolTip.delay: 1000
        ToolTip.text:  _tr.i18n("Click on the button, then enter the shortcut like you would in the program.\nExample for Ctrl+A: hold the Ctrl key and press A.")
        ToolTip.timeout: 5000

        onCheckedChanged: {
            if (checked) {
                mainButton.forceActiveFocus()
                _helper.startRecording()
            }
        }

        onFocusChanged: {
            if (!focus) {
                mainButton.checked = false
            }
        }
    }

    Button {
        id: clearButton
        Layout.fillHeight: true
        Layout.preferredWidth: height
        onClicked: root.keySequence = ""

        //icon name determines the direction of the arrow, NOT the direction of the app layout
        icon.name: Qt.application.layoutDirection == Qt.LeftToRight ? "edit-clear-locationbar-rtl" : "edit-clear-locationbar-ltr"
    }
}
