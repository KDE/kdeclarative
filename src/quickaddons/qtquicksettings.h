/*
    SPDX-FileCopyrightText: 2016 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLASMAQTQUICKSETTINGS_H
#define PLASMAQTQUICKSETTINGS_H

#include "quickaddons_export.h"

#if QUICKADDONS_ENABLE_DEPRECATED_SINCE(5, 93)
namespace KQuickAddons
{
/**
 * @namespace KQuickAddons::QtQuickSettings
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
 * @deprecated since 5.93, With Plasma 5.25 the QtQuickSettings are applied as part of the Plasma integration plugin
 */
QUICKADDONS_DEPRECATED_VERSION(5, 93, "With Plasma 5.25 the QtQuickSettings are applied as part of the Plasma integration plugin")
QUICKADDONS_EXPORT void init();

}
}

#endif
#endif
