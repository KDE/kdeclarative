/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KCMCONTROLSPLUGIN_H
#define KCMCONTROLSPLUGIN_H

#include <QQmlExtensionPlugin>

class KCMControlsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif
