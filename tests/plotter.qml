/*
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2015 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
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
