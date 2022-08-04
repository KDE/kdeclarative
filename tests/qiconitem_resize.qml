import org.kde.kquickcontrolsaddons 2.0
import QtQuick 2.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Rectangle {
    color: "white"
    width: 400
    height: 400

    QIconItem {
        id: anIcon
        icon: "rating"
        width: 8
        height: 8
    }

    Button {
        anchors.bottom: parent.bottom
        text: "Resize"
        onClicked: {
            anIcon.width += 4
            anIcon.height += 4
        }
    }
}
