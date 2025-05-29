/*
    SPDX-FileCopyrightText: 2024 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "enums.h"

#include <qqmlregistration.h>

#include <KKeySequenceRecorder>

namespace Foo
{
Q_NAMESPACE
QML_NAMED_ELEMENT(ShortcutType)
QML_FOREIGN_NAMESPACE(KeySequenceEnums)
};

class KKeySequenceRecorderForeign
{
    Q_GADGET
    QML_NAMED_ELEMENT(ShortcutPattern)
    QML_FOREIGN(KKeySequenceRecorder)
    QML_UNCREATABLE("Only for enums")
};
