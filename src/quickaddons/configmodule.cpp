/*
   This file is part of the KDE libraries

   Copyright (c) 1999 Matthias Hoelzer-Kluepfel <hoelzer@kde.org>
   Copyright (c) 2001 Michael Goffioul <kdeprint@swing.be>
   Copyright (C) 2004 Frans Englich <frans.englich@telia.com>
   Copyright (C) 2009 Dario Freddi <drf@kde.org>
   Copyright (C) 2015 Marco Martin <mart@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

*/

#include "configmodule.h"

#include <QTimer>
#include <QDebug>
#include <QUrl>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlEngine>

#include <kaboutdata.h>
#include <klocalizedstring.h>
#include <kdeclarative/qmlobject.h>
#include <kdeclarative/qmlobjectsharedengine.h>

#include <KPackage/Package>
#include <KPackage/PackageLoader>

namespace KQuickAddons {

class ConfigModulePrivate
{
public:
    ConfigModulePrivate(ConfigModule *module):
        _q(module),
        _qmlObject(nullptr),
        _buttons(ConfigModule::Help | ConfigModule::Default | ConfigModule::Apply),
        _about(nullptr),
        _useRootOnlyMessage(false),
        _needsAuthorization(false),
        _needsSave(false)
    {
    }

    void authStatusChanged(int status);

    ConfigModule *_q;
    KDeclarative::QmlObject *_qmlObject;
    ConfigModule::Buttons _buttons;
    const KAboutData *_about;
    QString _rootOnlyMessage;
    QString _quickHelp;
    bool _useRootOnlyMessage : 1;

    bool _needsAuthorization : 1;
    bool _needsSave  :1;
    QString _authActionName;

    static QHash<QObject *, ConfigModule *> s_rootObjects;
};

QHash<QObject *, ConfigModule *> ConfigModulePrivate::s_rootObjects = QHash<QObject *, ConfigModule *>();


ConfigModule::ConfigModule(const KAboutData *aboutData, QObject *parent, const QVariantList &)
    : QObject(parent), d(new ConfigModulePrivate(this))
{
    setAboutData(aboutData);
}

ConfigModule::ConfigModule(const KPluginMetaData &metaData, QObject *parent, const QVariantList &)
    : QObject(parent), d(new ConfigModulePrivate(this))
{
    KAboutData *aboutData = new KAboutData(metaData.pluginId(), metaData.name(), metaData.version(), metaData.description(), KAboutLicense::byKeyword(metaData.license()).key());

    for (auto author : metaData.authors()) {
        aboutData->addAuthor(author.name(), author.task(), author.emailAddress(), author.webAddress(), author.ocsUsername());
    }
    setAboutData(aboutData);
}

ConfigModule::ConfigModule(QObject *parent, const QVariantList &)
    : QObject(parent), d(new ConfigModulePrivate(this))
{
}

ConfigModule::~ConfigModule()
{
    //in case mainUi was never called
    if (d->_qmlObject) {
        ConfigModulePrivate::s_rootObjects.remove(d->_qmlObject->rootContext());
    }

    delete d->_qmlObject;
    delete d->_about;
    delete d;
}



ConfigModule *ConfigModule::qmlAttachedProperties(QObject *object)
{
    //at the moment of the attached object creation, the root item is the only one that hasn't a parent
    //only way to avoid creation of this attached for everybody but the root item
    const QQmlEngine *engine = QtQml::qmlEngine(object);
    QQmlContext *cont = QQmlEngine::contextForObject(object);

    //Search the qml context that is the "root" for the sharedqmlobject, which
    //is an ancestor of QQmlEngine::contextForObject(object) and the direct child
    //of the engine's root context: we can do this assumption on the internals as
    //we are distributed on the same repo.
    while (cont->parentContext() && cont->parentContext() != engine->rootContext()) {
        cont = cont->parentContext();
    }

    if (!object->parent() && ConfigModulePrivate::s_rootObjects.contains(cont)) {
        return ConfigModulePrivate::s_rootObjects.value(cont);
    } else {
        return nullptr;
    }
}

QQuickItem *ConfigModule::mainUi()
{
    if (d->_qmlObject) {
        return qobject_cast<QQuickItem *>(d->_qmlObject->rootObject());
    }

    // if we have a qml context, hook up to it and use its engine
    // this ensure that in e.g. Plasma config dialogs that use a different engine
    // so they can have different QtQuick Controls styles, we don't end up using
    // the shared engine that is used by the rest of plasma

    QQmlContext *ctx = QQmlEngine::contextForObject(this);

    if (ctx && ctx->engine()) {
        d->_qmlObject = new KDeclarative::QmlObject(ctx->engine(), ctx, this);
    } else {
        d->_qmlObject = new KDeclarative::QmlObjectSharedEngine(this);
    }

    ConfigModulePrivate::s_rootObjects[d->_qmlObject->rootContext()] = this;
    d->_qmlObject->setTranslationDomain(aboutData()->componentName());
    d->_qmlObject->setInitializationDelayed(true);

    KPackage::Package package = KPackage::PackageLoader::self()->loadPackage(QStringLiteral("KPackage/GenericQML"));
    package.setDefaultPackageRoot(QStringLiteral("kpackage/kcms"));
    package.setPath(aboutData()->componentName());

    if (!package.filePath("mainscript").isEmpty()) {
        d->_qmlObject->setSource(QUrl::fromLocalFile(package.filePath("mainscript")));
        d->_qmlObject->engine()->rootContext()->setContextProperty(QStringLiteral("kcm"), this);
        d->_qmlObject->completeInitialization();

        return qobject_cast<QQuickItem *>(d->_qmlObject->rootObject());
    } else {
        qWarning() << "Error loading the module" << aboutData()->componentName() << ": no QML file provided";
        return nullptr;
    }
}

ConfigModule::Buttons ConfigModule::buttons() const
{
    return d->_buttons;
}

void ConfigModule::setButtons(const KQuickAddons::ConfigModule::Buttons buttons)
{
    if (d->_buttons == buttons) {
        return;
    }

    d->_buttons = buttons;
    emit buttonsChanged();
}

void ConfigModule::setNeedsAuthorization(bool needsAuth)
{
    if (d->_needsAuthorization == needsAuth) {
        return;
    }

    d->_needsAuthorization = needsAuth;
    if (needsAuth && d->_about) {
        d->_authActionName = QStringLiteral("org.kde.kcontrol.") + d->_about->componentName() + QStringLiteral(".save");
        d->_needsAuthorization = true;

    } else {
        d->_authActionName = QString();
    }

    emit needsAuthorizationChanged();
    emit authActionNameChanged();
}

bool ConfigModule::needsAuthorization() const
{
    return d->_needsAuthorization;
}

QString ConfigModule::name() const
{
    return d->_about->displayName();
}

QString ConfigModule::description() const
{
    return d->_about->shortDescription();
}

void ConfigModule::setAuthActionName(const QString &name)
{
    if (d->_authActionName == name) {
        return;
    }

    d->_authActionName = name;
    d->_needsAuthorization = true;

    emit needsAuthorizationChanged();
    emit authActionNameChanged();
}

QString ConfigModule::authActionName() const
{
    return d->_authActionName;
}

QQmlEngine *ConfigModule::engine() const
{
    return d->_qmlObject->engine();
}

void ConfigModule::load()
{
    setNeedsSave(false);
}

void ConfigModule::save()
{
    setNeedsSave(false);
}

void ConfigModule::defaults()
{
}

const KAboutData *ConfigModule::aboutData() const
{
    return d->_about;
}

void ConfigModule::setAboutData(const KAboutData *about)
{
    if (about != d->_about) {
        delete d->_about;
        d->_about = about;
    }
}

void ConfigModule::setRootOnlyMessage(const QString &message)
{
    if (d->_rootOnlyMessage == message) {
        return;
    }

    d->_rootOnlyMessage = message;
    emit rootOnlyMessageChanged();
}

QString ConfigModule::rootOnlyMessage() const
{
    return d->_rootOnlyMessage;
}

void ConfigModule::setUseRootOnlyMessage(bool on)
{
    if (d->_useRootOnlyMessage == on) {
        return;
    }

    d->_useRootOnlyMessage = on;

    emit useRootOnlyMessageChanged();
}

bool ConfigModule::useRootOnlyMessage() const
{
    return d->_useRootOnlyMessage;
}

void ConfigModule::setQuickHelp(const QString &help)
{
    if (d->_quickHelp == help) {
        return;
    }

    d->_quickHelp = help;

    emit quickHelpChanged();
}

QString ConfigModule::quickHelp() const
{
    return d->_quickHelp;
}

void ConfigModule::setNeedsSave(bool needs)
{
    if (needs == d->_needsSave) {
        return;
    }

    d->_needsSave = needs;
    emit needsSaveChanged();
}

bool ConfigModule::needsSave()
{
    return d->_needsSave;
}

}

#include "moc_configmodule.cpp"

