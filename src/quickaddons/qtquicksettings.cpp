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

void KQuickAddons::QtQuickSettings::init()
{
    PlasmaQtQuickSettings::RendererSettings s;
    if (!s.renderLoop().isNull()) {
        qputenv("QSG_RENDER_LOOP", s.renderLoop().toLatin1());
    }

    if (!s.sceneGraphBackend().isNull()) {
        qputenv("QMLSCENE_DEVICE", s.sceneGraphBackend().toLatin1());
        //TODO Qt5.8 - QQuickWindow::setSceneGraphBackend(..)
    }
    if (s.forceGlCoreProfile()) {
        QSurfaceFormat format;
        format.setVersion(3,2);
        format.setProfile(QSurfaceFormat::CoreProfile);
        QSurfaceFormat::setDefaultFormat(format);
    }
}

