/*
 *   Copyright 2015 by Marco Martin <mart@kde.org>
 *   Copyright 2015 by David Edmundson <davidedmundson@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.0
import QtQuick.Controls 1.3

import org.kde.kquickcontrolsaddons 2.0

Item {
    width: 500
    height: 200

    Plotter {
        id: renderer
        anchors.fill: parent
        anchors.margins: 0
        stacked: stackedButton.checked
        autoRange: autoRangeButton.checked
        horizontalGridLineCount: linesSpinner.value

        dataSets: [
            PlotData {
                color: "#4cb2ff"
            },
            PlotData {
                color: "#00b200"
            }
        ]

    }
    Row {
        Button {
            text: "Add values"
            checkable: true
            Timer {
                interval: 100
                running: parent.checked
                repeat: true
                onTriggered: {
                    renderer.addSample([Math.random() * 40, Math.random() * 40])
                }
            }
        }

        Button {
            id: stackedButton
            text: "Stacked"
            checkable: true
            checked: true
        }
        Button {
            id: autoRangeButton
            text: "Auto Range"
            checkable: true
            checked: true
        }

        SpinBox {
            id: linesSpinner
            value: 5
            minimumValue: 0
        }
    }
}
