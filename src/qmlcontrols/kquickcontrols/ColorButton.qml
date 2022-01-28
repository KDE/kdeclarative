/*
    SPDX-FileCopyrightText: 2015 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

import QtQuick 2.2
import QtQuick.Controls 1.2 as QtControls
import QtQuick.Dialogs 1.0 as QtDialogs

/**
 * @short A pushbutton to display or allow user selection of a color.
 *
 * This widget can be used to display or allow user selection of a color.
 *
 * Example usage:
 * @code
 * import org.kde.kquickcontrols 2.0
 *
 * ColorButton {
 *   [...]
 *   onColorChanged: console.log(color)
 * }
 * @endcode
 *
 * @inherits QtQuick.Controls.Button
 */
QtControls.Button {
    id: colorPicker

    /**
     * The user selected color
     */
    property alias color: colorDialog.color

    /**
     * Title to show in the dialog
     */
    property alias dialogTitle: colorDialog.title

    /**
     * The color which the user has currently selected whilst the dialog is open
     * For the color that is set when the dialog is accepted, use the color property.
     */
    property alias currentColor: colorDialog.currentColor

    /**
     * Allow the user to configure an alpha value
     */
    property alias showAlphaChannel: colorDialog.showAlphaChannel

    /**
     * This signal is emitted when the color dialog has been accepted
     *
     * @since 5.61
     */
    signal accepted(color color)

    readonly property real _buttonMarigns: 4 // same as QStyles. Remove if we can get this provided by the QQC theme

    implicitWidth: 40 + _buttonMarigns*2 //to perfectly clone kcolorbutton from kwidgetaddons

    Accessible.name: i18nc("@info:whatsthis for a button", "Color button")
    Accessible.description: enabled ?
        i18nc("@info:whatsthis for a button of current color code %1", "Current color is %1. This button will open a color chooser dialog.", color)
      : i18nc("@info:whatsthis for a button of current color code %1", "Current color is %1.", color)

    //create a checkerboard background for alpha to be adjusted
    Canvas {
        anchors.fill: colorBlock
        visible: colorDialog.color.a < 1

        onPaint: {
            var ctx = getContext('2d');

            ctx.fillStyle = "white";
            ctx.fillRect(0,0, ctx.width, ctx.height)

            ctx.fillStyle = "black";
            //in blocks of 16x16 draw two black squares of 8x8 in top left and bottom right
            for (var j=0;j<width;j+=16) {
                for (var i=0;i<height;i+=16) {
                    //top left, bottom right
                    ctx.fillRect(j,i,8,8);
                    ctx.fillRect(j+8,i+8,8,8);
                }
            }
        }

    }

    Rectangle {
        id: colorBlock

        anchors.centerIn: parent
        height: parent.height - _buttonMarigns*2
        width: parent.width - _buttonMarigns*2


        color: enabled ? colorDialog.color : disabledPalette.button

        SystemPalette {
            id: disabledPalette
            colorGroup: SystemPalette.Disabled
        }
    }

    QtDialogs.ColorDialog {
        id: colorDialog
        onAccepted: colorPicker.accepted(color)
    }

    onClicked: {
        colorDialog.open()
    }
}
