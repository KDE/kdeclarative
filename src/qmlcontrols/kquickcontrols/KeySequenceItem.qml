import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import org.kde.private.kquickcontrols 2.0 as KQuickControlsPrivate

RowLayout {
    id: root

    property alias showClearButton: clearButton.visible
    property alias modifierlessAllowed: helper.modifierlessAllowed
    property alias multiKeyShortcutsAllowed: helper.multiKeyShortcutsAllowed
    // Can't use proper types for QGadgets
    property var/*QKeySequence*/ keySequence: helper.fromString()

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
    property alias checkForConflictsAgainst: helper.checkAgainstShortcutTypes

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
        id: helper
        onGotKeySequence: keySequence => {
            if (isKeySequenceAvailable(keySequence)) {
                root.keySequence = keySequence;
            }
            mainButton.checked = false;
            root.captureFinished();
        }
    }

    KQuickControlsPrivate.TranslationContext {
        id: _tr
        domain: "kdeclarative5"
    }

    Button {
        id: mainButton

        icon.name: "configure"

        checkable: true
        focus: checked

        hoverEnabled: true

        text: {
            const keys = helper.isRecording ? helper.currentKeySequence : root.keySequence
            const text = helper.keySequenceIsEmpty(keys)
                ? (helper.isRecording
                    ? _tr.i18nc("What the user inputs now will be taken as the new shortcut", "Input")
                    : _tr.i18nc("No shortcut defined", "None"))
                // Single ampersand gets interpreted by the button as a mnemonic
                // and removed; replace it with a double ampersand so that it
                // will be displayed by the button as a single ampersand, or
                // else shortcuts with the actual ampersand character will
                // appear to be partially empty.
                : helper.keySequenceNativeText(keys).replace('&', '&&');
            // These spaces are intentional
            return " " + text + (helper.isRecording ? " ... " : " ")
        }

        ToolTip.visible: hovered
        ToolTip.delay: 1000
        ToolTip.text: _tr.i18n("Click on the button, then enter the shortcut like you would in the program.\nExample for Ctrl+A: hold the Ctrl key and press A.")
        ToolTip.timeout: 5000

        onCheckedChanged: {
            if (checked) {
                helper.window = helper.renderWindow(parent.Window.window)
                mainButton.forceActiveFocus()
                helper.startRecording()
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
        onClicked: root.keySequence = helper.fromString()

        // icon name determines the direction of the arrow, NOT the direction of the app layout
        icon.name: Qt.application.layoutDirection === Qt.LeftToRight ? "edit-clear-locationbar-rtl" : "edit-clear-locationbar-ltr"
    }
}
