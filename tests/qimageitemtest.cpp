/*
 *   Copyright 2011 Marco Martin <mart@gmail.com>
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
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>

#include <QtTest/qtestcase.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QQuickView view;
    QQmlContext *context = view.rootContext();

    QImage image;
    if (app.devicePixelRatio() < 2) {
        image = QImage(QFINDTESTDATA("testimage.png"));
    } else {
        image = QImage(QFINDTESTDATA("testimage@2x.png"));
        image.setDevicePixelRatio(2);
    }

    context->setContextProperty(QStringLiteral("testImage"),
                                image);


    view.setSource(QUrl::fromLocalFile(QFINDTESTDATA("qimageitemtest.qml")));
    view.show();

    return app.exec();
}
