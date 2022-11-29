import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import org.kde.private.kquickcontrols 2.0 as KQuickControlsPrivate

RowLayout {
    id: root

    property bool showClearButton: true
    property bool showCancelButton: false /// TODO KF6 default to true
    property alias modifierOnlyAllowed: helper.modifierOnlyAllowed
    property alias modifierlessAllowed: helper.modifierlessAllowed
    property alias multiKeyShortcutsAllowed: helper.multiKeyShortcutsAllowed
    property alias keySequence: helper.currentKeySequence

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
     * @deprecated Use keySequenceModified()
     */
    signal captureFinished()

    /***
     * Emitted whenever the key sequence is modified by the user, interacting with the component
     *
     * Either by interacting capturing a key sequence or pressing the clear button.
     *
     * @since 5.99
     */
    signal keySequenceModified()

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
            } else {
                root.keySequence = mainButton.previousSequence
            }
            mainButton.checked = false;
            root.captureFinished();
            root.keySequenceModified();
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
        property var previousSequence: ""

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

        Accessible.description: _tr.i18n("Click on the button, then enter the shortcut like you would in the program.\nExample for Ctrl+A: hold the Ctrl key and press A.")

        ToolTip {
            visible: mainButton.hovered
            text: mainButton.Accessible.description
        }

        onCheckedChanged: {
            if (checked) {
                previousSequence = helper.keySequenceNativeText(root.keySequence)
                helper.window = helper.renderWindow(parent.Window.window)
                mainButton.forceActiveFocus()
                helper.startRecording()
            } else if (helper.isRecording) {
                helper.cancelRecording()
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
        visible: root.showClearButton && !helper.isRecording
        onClicked: {
            root.keySequence = helper.fromString()
            root.keySequenceModified();
            root.captureFinished(); // Not really capturing, but otherwise we cannot track this state, hence apps should use keySequenceModified
        }

        // Just a helper object
        Text {
            id: theText
            visible: false
            text: root.keySequence
        }
        enabled: theText.text.length > 0

        hoverEnabled: true
        // icon name determines the direction of the arrow, NOT the direction of the app layout
        icon.name: Qt.application.layoutDirection === Qt.LeftToRight ? "edit-clear-locationbar-rtl" : "edit-clear-locationbar-ltr"

        Accessible.name: _tr.i18nc("@info:tooltip", "Clear Key Sequence")

        ToolTip {
            visible: clearButton.hovered
            text: clearButton.Accessible.name
        }
    }

    Button {
        Layout.fillHeight: true
        Layout.preferredWidth: height
        onClicked: helper.cancelRecording()
        visible: root.showCancelButton && helper.isRecording

        icon.name: "dialog-cancel"

        Accessible.name: _tr.i18nc("@info:tooltip", "Cancel Key Sequence Recording")

        ToolTip {
            visible: parent.hovered
            text: parent.Accessible.name
        }
    }
}
