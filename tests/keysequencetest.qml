import QtQuick 2.0

import org.kde.kquickcontrols 2.0
import QtQuick.Layouts 1.1

RowLayout {

    KeySequenceItem
    {
        id: sequenceItem
        patterns: ShortcutPattern.Modifier | ShortcutPattern.ModifierAndKey
    }
    Text
    {
        text: sequenceItem.keySequence
    }
}
