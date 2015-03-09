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

#include <QLayout>
#include <QTimer>
#include <QDebug>
#include <QUrl>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlEngine>

#include <kaboutdata.h>
#include <klocalizedstring.h>
#include <kauthexecutejob.h>
#include <kdeclarative/qmlobject.h>

#include <KPackage/Package>
#include <KPackage/PackageLoader>

namespace KDeclarative {

class ConfigModulePrivate
{
public:
    ConfigModulePrivate():
        _buttons(ConfigModule::Help | ConfigModule::Default | ConfigModule::Apply),
        _about(0),
        _useRootOnlyMessage(false),
        _needsAuthorization(false),
        _needsSave(false),
        _authAction()
    { }

    void authStatusChanged(int status);

    QmlObject *_qmlObject;
    ConfigModule::Buttons _buttons;
    const KAboutData *_about;
    QString _rootOnlyMessage;
    QString _quickHelp;
    QString m_ExportText;
    bool _useRootOnlyMessage : 1;

    bool _needsAuthorization : 1;
    bool _needsSave  :1;
    KAuth::Action _authAction;
};

ConfigModule::ConfigModule(const KAboutData *aboutData, QObject *parent, const QVariantList &)
    : QObject(parent), d(new ConfigModulePrivate)
{
    setAboutData(aboutData);
}

ConfigModule::ConfigModule(QObject *parent, const QVariantList &)
    : QObject(parent), d(new ConfigModulePrivate)
{
}

QQuickItem *ConfigModule::mainUi()
{
    if (d->_qmlObject) {
        return qobject_cast<QQuickItem *>(d->_qmlObject->rootObject());
    }
    d->_qmlObject = new KDeclarative::QmlObject(this);
    d->_qmlObject->setTranslationDomain(aboutData()->componentName());
    d->_qmlObject->setInitializationDelayed(true);

    KPackage::Package package = KPackage::PackageLoader::self()->loadPackage("KPackage/GenericQML");
    package.setDefaultPackageRoot("kpackage/kcms");
    package.setPath(aboutData()->componentName());

    d->_qmlObject->setSource(QUrl::fromLocalFile(package.filePath("mainscript")));
    d->_qmlObject->engine()->rootContext()->setContextProperty("kcm", this);
    d->_qmlObject->completeInitialization();

    return qobject_cast<QQuickItem *>(d->_qmlObject->rootObject());
}

ConfigModule::Buttons ConfigModule::buttons() const
{
    return d->_buttons;
}

void ConfigModule::setButtons(Buttons buttons)
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
        d->_authAction = KAuth::Action(QString("org.kde.kcontrol." + d->_about->componentName() + ".save"));
        d->_needsAuthorization = d->_authAction.isValid();
        d->_authAction.setHelperId("org.kde.kcontrol." + d->_about->componentName());
//FIXME
//        d->_authAction.setParentWidget(this);
        authStatusChanged(d->_authAction.status());
    } else {
        d->_authAction = KAuth::Action();
    }

    emit needsAuthorizationChanged();
}

bool ConfigModule::needsAuthorization() const
{
    return d->_needsAuthorization;
}

void ConfigModule::setAuthAction(const KAuth::Action &action)
{
    if (!d->_authAction.isValid()) {
        qWarning() << "Auth action" << action.name() << "is invalid";
        d->_needsAuthorization = false;
        return;
    }
    d->_authAction = action;
    d->_needsAuthorization = true;
//FIXME
//    d->_authAction.setParentWidget(this);
    authStatusChanged(d->_authAction.status());
}

KAuth::Action ConfigModule::authAction() const
{
    return d->_authAction;
}

QQmlEngine *ConfigModule::engine() const
{
    return d->_qmlObject->engine();
}

void ConfigModule::authStatusChanged(KAuth::Action::AuthStatus status)
{
    switch (status) {
    case KAuth::Action::AuthorizedStatus:
        setUseRootOnlyMessage(false);
        break;
    case KAuth::Action::AuthRequiredStatus:
        setUseRootOnlyMessage(true);
        setRootOnlyMessage(i18n("You will be asked to authenticate before saving"));
        break;
    default:
        setUseRootOnlyMessage(true);
        setRootOnlyMessage(i18n("You are not allowed to save the configuration"));
        break;
    }

    qDebug() << useRootOnlyMessage();
}

ConfigModule::~ConfigModule()
{
    delete d->_about;
    delete d;
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

QString ConfigModule::exportText() const
{
    return d->m_ExportText;
}

void ConfigModule::setExportText(const QString &text)
{
    if (d->m_ExportText == text) {
        return;
    }

    d->m_ExportText = text;
    emit exportTextChanged();
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

