/*
    SPDX-FileCopyrightText: 2014 Antonis Tsiapaliokas <antonis.tsiapaliokas@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KIOPLUGIN_H
#define KIOPLUGIN_H

#include <QQmlExtensionPlugin>

class KioPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif
