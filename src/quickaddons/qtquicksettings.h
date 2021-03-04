/*
    SPDX-FileCopyrightText: 2016 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLASMAQTQUICKSETTINGS_H
#define PLASMAQTQUICKSETTINGS_H

#include "quickaddons_export.h"

namespace KQuickAddons
{
/**
 * @namespace KQuickAddons::QtQuickSettings
 *
 * TODO
 */
namespace QtQuickSettings
{
/**
 * Load settings from the appConfig (app + kdeglobals) which relate to QtQuick rendering.
 * This will then set relevant default QWindow and QSurfaceFormat flags, or in some cases environment variables
 *
 * This function must be called at the start of your application before any windows are created,
 * but after an instance of QGuiApplication is already available.
 *
 * @since 5.26
 */
QUICKADDONS_EXPORT void init();

}
}

#endif
