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

#include "qtquicksettings.h"
#include "renderersettings.h"

#include <QSurfaceFormat>
#include <QQuickWindow>
#include <QLibraryInfo>
#include <QVersionNumber>

void KQuickAddons::QtQuickSettings::init()
{
    PlasmaQtQuickSettings::RendererSettings s;
    if (!s.renderLoop().isEmpty()) {
        qputenv("QSG_RENDER_LOOP", s.renderLoop().toLatin1());
    }

    if (!s.sceneGraphBackend().isNull()) {
     QQuickWindow::setSceneGraphBackend(s.sceneGraphBackend());
    }
    auto format = QSurfaceFormat::defaultFormat();
    if (s.forceGlCoreProfile()) {
        format.setVersion(3,2);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    // Before Qt 5.12.2 this setting was somewhat unstable
    // it was opt-in to find bugs both in KDE and Qt
    // For 5.13 with modern plasma it should be fine
    if (s.graphicsResetNotifications() ||
        QLibraryInfo::version() >= QVersionNumber(5, 13, 0)) {
        format.setOption(QSurfaceFormat::ResetNotification);
    }
    QSurfaceFormat::setDefaultFormat(format);
}

