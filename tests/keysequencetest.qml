import QtQuick 2.0

import org.kde.kquickcontrols 2.0
import QtQuick.Layouts 1.1

RowLayout {

    KeySequenceItem
    {
        id: sequenceItem
        patterns: ShortcutPattern.Modifier | ShortcutPattern.Key
    }
    Text
    {
        text: sequenceItem.keySequence
    }
}
