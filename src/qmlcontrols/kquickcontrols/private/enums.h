/*
    SPDX-FileCopyrightText: 2014 David Edmundson <davidedmundson@kde.org>
    SPDX-FileCopyrightText: 2020 David Redondo <kde@david-redondo.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include "kquickcontrolsprivate_export.h"

#include <QObject>

namespace KeySequenceEnums
{

Q_NAMESPACE_EXPORT(KQUICKCONTROLSPRIVATE_EXPORT)
enum ShortcutType {
    None = 0x00, //!< No checking for conflicts
    StandardShortcuts = 0x01, //!< Check against standard shortcuts. @see KStandardShortcut
    GlobalShortcuts = 0x02, //!< Check against global shortcuts. @see KGlobalAccel
};
Q_ENUM_NS(ShortcutType);
Q_DECLARE_FLAGS(ShortcutTypes, ShortcutType)
Q_FLAG_NS(ShortcutTypes)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KeySequenceEnums::ShortcutTypes)
