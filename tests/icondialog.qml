/*
    SPDX-FileCopyrightText: 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1

import org.kde.kquickcontrolsaddons 2.0

RowLayout {
    spacing: 10

    IconDialog {
        id: iconDialog
        iconSize: Number(iconSizeInput.text)
        title: dialogTitleInput.text
        user: userCheckbox.checked
        customLocation: customLocationField.text
        modality: {
            switch(modalityCombo.currentIndex) {
            case 0: return Qt.WindowModal
            case 1: return Qt.ApplicationModal
            case 2: return Qt.NonModal
            }
        }
    }

    Button {
        Layout.minimumWidth: 80
        Layout.maximumWidth: Layout.minimumWidth
        Layout.minimumHeight: Layout.minimumWidth
        Layout.maximumHeight: Layout.minimumWidth
        icon.name: iconDialog.iconName
        onClicked: iconDialog.open()
    }

    GridLayout {
        Layout.fillWidth: true
        columns: 2

        Label {
            Layout.columnSpan: 2
            font.pointSize: Math.round(someLabel.font.pointSize * 1.2)
            text: "Push the button to open the icon dialog"
        }

        Label {
            id: someLabel
            text: "Dialog visible:"
        }
        Label {
            text: iconDialog.visible ? "yes" : "no"
        }

        Label {
            text: "Selected icon:"
        }
        Label {
            text: iconDialog.iconName || "(none)"
        }

        Label {
            text: "Desired icon size (blank = default):"
        }
        TextField {
            id: iconSizeInput
            text: ""
        }

        Label {
            text: "Dialog title:"
        }
        TextField {
            id: dialogTitleInput
            Layout.minimumWidth: 250
        }

        Label {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            font.pointSize: Math.round(someLabel.font.pointSize / 1.2)
            opacity: 0.6
            wrapMode: Text.WordWrap
            text: "Not setting a title at all will default to 'Choose Icon' "
                + "whereas explicitly setting it to empty (which we do here) "
                + "will default to the application name"
        }

        CheckBox {
            id: userCheckbox
            Layout.columnSpan: 2
            text: "Begin with user icons rather than system icons"
        }

        Label {
            text: "Custom search location"
        }

        TextField {
            id: customLocationField
            Layout.minimumWidth: 250
        }

        Label {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            font.pointSize: Math.round(someLabel.font.pointSize / 1.2)
            opacity: 0.6
            wrapMode: Text.WordWrap
            text: "This needs to be a URL to a local folder. You should enable "
                + "'Begin with user icons' when setting this"
        }

        Label {
            text: "Modality"
        }

        ComboBox {
            id: modalityCombo
            Layout.minimumWidth: 250
            model: ["Window modal (default)", "Application modal", "Non-modal"]
        }

    }
}
