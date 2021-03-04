/*
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QApplication>

#include <KLocalizedString>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QQuickItem>

#include <KAboutData>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlExpression>
#include <QQmlProperty>
#include <QQuickWindow>
#include <QStandardPaths>
#include <kdeclarative/qmlobject.h>
#include <kpackage/package.h>
#include <kpackage/packageloader.h>

int main(int argc, char **argv)
{
    QCommandLineParser parser;
    QApplication app(argc, argv);

    const QString description = i18n("KPackage QML application shell");

    app.setApplicationVersion(QStringLiteral("0.1"));
    parser.addVersionOption();
    parser.addHelpOption();
    parser.setApplicationDescription(description);

    QCommandLineOption appPluginOption(QCommandLineOption(QStringList() << QStringLiteral("a") << QStringLiteral("app"),
                                                          i18n("The unique name of the application (mandatory)"),
                                                          QStringLiteral("app")));

    parser.addOption(appPluginOption);

    parser.process(app);

    QString packagePath;
    if (parser.isSet(appPluginOption)) {
        packagePath = parser.value(appPluginOption);
    } else {
        parser.showHelp(1);
    }

    // usually we have an ApplicationWindow here, so we do not need to create a window by ourselves
    KDeclarative::QmlObject obj;
    obj.setTranslationDomain(packagePath);
    obj.setInitializationDelayed(true);
    obj.loadPackage(packagePath);
    if (!obj.package().isValid()) {
        qWarning() << "The specified Application package is not valid.";
        return 1;
    }
    obj.engine()->rootContext()->setContextProperty(QStringLiteral("commandlineArguments"), parser.positionalArguments());
    QVariantMap paths;
    paths[QStringLiteral("desktop")] = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    paths[QStringLiteral("documents")] = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    paths[QStringLiteral("music")] = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    paths[QStringLiteral("movies")] = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    paths[QStringLiteral("pictures")] = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    paths[QStringLiteral("home")] = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    obj.engine()->rootContext()->setContextProperty(QStringLiteral("userPaths"), paths);
    obj.completeInitialization();

    if (!obj.package().metadata().isValid()) {
        return -1;
    }

    KPluginMetaData data = obj.package().metadata();
    // About data
    KAboutData aboutData(data.pluginId(), data.name(), data.version(), data.description(), KAboutLicense::byKeyword(data.license()).key());

    const auto authors = data.authors();
    for (auto &author : authors) {
        aboutData.addAuthor(author.name(), author.task(), author.emailAddress(), author.webAddress(), author.ocsUsername());
    }
    // We assume that desktop file in applications dir is named similar as
    // the pluginId, this is required to get icons working on wayland for example
    aboutData.setDesktopFileName(data.pluginId());

    KAboutData::setApplicationData(aboutData);

    // The root is not a window?
    // have to use a normal QQuickWindow since the root item is already created
    QQuickItem *item = qobject_cast<QQuickItem *>(obj.rootObject());
    QWindow *window = qobject_cast<QWindow *>(obj.rootObject());
    if (item) {
        QQuickWindow view;
        item->setParentItem(view.contentItem());
        if (item->implicitWidth() > 0 && item->implicitHeight() > 0) {
            view.resize(item->implicitWidth(), item->implicitHeight());
        } else {
            view.resize(item->width(), item->height());
        }

        // set anchors
        QQmlExpression expr(obj.engine()->rootContext(), obj.rootObject(), QStringLiteral("parent"));
        QQmlProperty prop(obj.rootObject(), QStringLiteral("anchors.fill"));
        prop.write(expr.evaluate());
        view.setTitle(obj.package().metadata().name());
        view.setIcon(QIcon::fromTheme(obj.package().metadata().iconName()));

        // Forward Qt.quit() in QML code to QApplication::quit()
        QObject::connect(obj.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);

        view.show();
        return app.exec();
    } else if (window) {
        window->setTitle(obj.package().metadata().name());
        window->setIcon(QIcon::fromTheme(obj.package().metadata().iconName()));
    } else {
        qWarning() << "The root Qml Item should be either a kind of window or a QQuickItem";
        return 1;
    }

    return app.exec();
}
