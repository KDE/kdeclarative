/*
 *   Copyright 2014 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
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

#include <QGuiApplication>

#include <klocalizedstring.h>
#include <qcommandlineparser.h>
#include <qcommandlineoption.h>

#include <kpackage/package.h>
#include <kpackage/packageloader.h>
#include <QQmlContext>

#include "view.h"

int main(int argc, char **argv)
{
    QCommandLineParser parser;
    QGuiApplication app(argc, argv);

    const QString description = i18n("KPackage QML application shell");
    const char version[] = "0.1";

    app.setApplicationVersion(version);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.setApplicationDescription(description);

    QCommandLineOption appPluginOption(QCommandLineOption(QStringList() << "a" << "app", i18n("The unique name of the application (mandatory)"), "app"));

    parser.addOption(appPluginOption);

    parser.process(app);

    QString packagePath;
    if (parser.isSet(appPluginOption)) {
        packagePath = parser.value(appPluginOption);
    } else {
        parser.showHelp(1);
    }

    View view(packagePath, parser.positionalArguments());
    view.rootContext()->setContextProperty("application", &view);
    view.show();

    return app.exec();
}

