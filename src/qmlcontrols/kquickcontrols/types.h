/*
    SPDX-FileCopyrightText: 2024 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "enums.h"

#include <qqmlregistration.h>

namespace Foo
{
Q_NAMESPACE
QML_NAMED_ELEMENT(ShortcutType)
QML_FOREIGN_NAMESPACE(KeySequenceEnums)
};
