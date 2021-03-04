/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>

#include <kdeclarative/kdeclarative.h>

#include <qtestcase.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    QQmlContext *context = view.rootContext();
    context->setContextProperty(QStringLiteral("backgroundColor"), QColor(Qt::yellow));

    KDeclarative::KDeclarative::setupEngine(view.engine());
    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(view.engine());

    view.setSource(QUrl::fromLocalFile(QFINDTESTDATA("test.qml")));
    view.show();

    return app.exec();
}
