/*
    SPDX-FileCopyrightText: 2017 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: MIT
*/

#include "kcmcontrolsplugin.h"

#include "settinghighlighterprivate.h"
#include "settingstateproxy.h"

void KCMControlsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QByteArray("org.kde.kcm"));

    qmlRegisterType<SettingStateProxy>(uri, 1, 3, "SettingStateProxy");
    qmlRegisterType<SettingHighlighterPrivate>("org.kde.kcm.private", 1, 3, "SettingHighlighterPrivate");
}
