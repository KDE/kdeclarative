/*
    SPDX-FileCopyrightText: 2017 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: MIT
*/

#include "kcmcontrolsplugin.h"

#include "settinghighlighterprivate.h"
#include "settingstateproxy.h"
#include <KQuickAddons/ConfigModule>

void KCMControlsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QByteArray("org.kde.kcm"));

    qmlRegisterUncreatableType<KQuickAddons::ConfigModule>(uri, 1, 0, "ConfigModule", QLatin1String("Do not create objects of type ConfigModule"));
    qmlRegisterType<SettingStateProxy>(uri, 1, 3, "SettingStateProxy");
    qmlRegisterType<SettingHighlighterPrivate>("org.kde.kcm.private", 1, 3, "SettingHighlighterPrivate");
}
