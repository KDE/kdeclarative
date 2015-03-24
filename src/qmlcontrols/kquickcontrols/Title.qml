import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import org.kde.kquickcontrolsaddons 2.0

/**
 * Add a title that follows the application theme
 * Similar to KTitleWidget
 */

RowLayout {
    /**
     * Text in the title
     */
    property alias text: textLabel.text

    /**
     * Optional icon to show to the right of the title
     * This can be a string of an icon name or a QIcon object
     */
    property alias icon: iconLabel.icon

    LayoutMirroring.enabled: Qt.application.layoutDirection == Qt.RightToLeft

    Label {
        id: textLabel
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter

        Component.onCompleted: {
            font.pointSize = font.pointSize * 1.4 // to match KTitleWidget
        }
    }

    QIconItem {
        id: iconLabel
        Layout.alignment: Qt.AlignVCenter
        Layout.preferredWidth: implicitWidth
        Layout.preferredHeight: implicitHeight
    }
}