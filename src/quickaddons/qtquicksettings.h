/*
 *   Copyright 2016 David Edmundson <davidedmundson@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
     * This function must be called at the start of your application before any windows are created.
     *
     * @since 5.26
     */
    QUICKADDONS_EXPORT void init();

    }
}

#endif
