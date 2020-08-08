/*
    SPDX-FileCopyrightText: 2016 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
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

