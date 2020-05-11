import QtQuick 2.0
import QtQuick.Controls 1.1

import org.kde.kquickcontrols 2.0
import QtQuick.Layouts 1.1

RowLayout {

    KeySequenceItem
    {
        id: sequenceItem

    }
    Text
    {
        text: sequenceItem.keySequence
    }
}
