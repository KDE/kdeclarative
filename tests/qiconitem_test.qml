import org.kde.kquickcontrolsaddons 2.0
import QtQuick 2.2

GridView {
    width: 300
    cellWidth: 16
    cellHeight: 16

    model: 50000
    delegate: QIconItem {
        width: 16
        height: 16
        icon: "rating"
    }
}
