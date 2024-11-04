/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef DRAGANDDROPPLUGIN_H
#define DRAGANDDROPPLUGIN_H

#include <QQmlExtensionPlugin>

/*!
 * \qmltype DragAndDropPlugin
 * \inqmlmodule org.kde.draganddrop
 */
class DragAndDropPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    /*! \qmlmethod void DragAndDropPlugin::registerTypes(const char *uri) */
    void registerTypes(const char *uri) override;
};

#endif
