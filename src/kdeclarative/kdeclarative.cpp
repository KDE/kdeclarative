/*
 *   Copyright 2011 Marco Martin <mart@kde.org>
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

#include "kdeclarative.h"
#include "private/kdeclarative_p.h"
#include "private/kiconprovider_p.h"
#include "private/kioaccessmanagerfactory_p.h"
#include "qmlobject.h"

#include <QCoreApplication>
#include <QDebug>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlExpression>
#include <QtQml/QQmlDebuggingEnabler>

#include <kconfiggroup.h>
#include <ksharedconfig.h>

namespace KDeclarative {

QStringList KDeclarativePrivate::s_runtimePlatform;

KDeclarativePrivate::KDeclarativePrivate()
    : contextObj(nullptr)
{
}

KDeclarative::KDeclarative()
    : d(new KDeclarativePrivate)
{
}

KDeclarative::~KDeclarative()
{
    delete d;
}

void KDeclarative::setDeclarativeEngine(QQmlEngine *engine)
{
    if (d->declarativeEngine.data() == engine) {
        return;
    }
    d->declarativeEngine = engine;
}

QQmlEngine *KDeclarative::declarativeEngine() const
{
    return d->declarativeEngine.data();
}

#ifndef KDECLARATIVE_NO_DEPRECATED
void KDeclarative::initialize()
{
}
#endif

void KDeclarative::setupBindings()
{
    //get rid of stock network access manager factory
    QQmlNetworkAccessManagerFactory *factory = d->declarativeEngine.data()->networkAccessManagerFactory();
    d->declarativeEngine.data()->setNetworkAccessManagerFactory(nullptr);
    delete factory;
    d->declarativeEngine.data()->setNetworkAccessManagerFactory(new KIOAccessManagerFactory());

    /*Create a context object for the root qml context.
      in this way we can register global functions, in this case the i18n() family*/
    if (!d->contextObj) {
        d->contextObj = new KLocalizedContext(d->declarativeEngine.data());
    }

    //If the engine is in a qmlObject take the qmlObject rootContext instead of the engine one.
    if (d->qmlObj) {
        d->qmlObj->rootContext()->setContextObject(d->contextObj);
    } else {
        d->declarativeEngine.data()->rootContext()->setContextObject(d->contextObj);
    }

    if (!d->translationDomain.isNull()) {
        d->contextObj->setTranslationDomain(d->translationDomain);
    }

    /* Tell the engine to search for platform-specific imports first
       (so it will "win" in import name resolution).
       addImportPath adds the path at the beginning, so to honour user's
       paths we need to traverse the list in reverse order */
    const QStringList pluginPathList = d->declarativeEngine.data()->importPathList();

    const QString target = componentsTarget();
    if (target != defaultComponentsTarget()) {
        const QStringList paths = pluginPathList;
        QStringListIterator it(paths);
        it.toBack();
        while (it.hasPrevious()) {
            QString path = it.previous();
            path = path.left(path.lastIndexOf(QLatin1Char('/')));
            d->declarativeEngine.data()->addImportPath(path + QStringLiteral("/platformqml/") + target);
        }
    }

    // setup ImageProvider for KDE icons
    d->declarativeEngine.data()->addImageProvider(QStringLiteral("icon"), new KIconProvider);
}

void KDeclarative::setTranslationDomain(const QString &translationDomain)
{
    d->translationDomain = translationDomain;
    if (d->contextObj) {
        d->contextObj->setTranslationDomain(d->translationDomain);
    }
}

QString KDeclarative::translationDomain() const
{
    return d->translationDomain;
}

void KDeclarative::setupQmlJsDebugger()
{
    if (QCoreApplication::arguments().contains(QLatin1String("-qmljsdebugger"))) {
        QQmlDebuggingEnabler enabler;
    }
}

QString KDeclarative::defaultComponentsTarget()
{
    return QLatin1String("desktop");
}

QString KDeclarative::componentsTarget()
{
    const QStringList platform = runtimePlatform();
    if (platform.isEmpty()) {
        return defaultComponentsTarget();
    }

    return platform.last();
}

QStringList KDeclarative::runtimePlatform()
{
    if (KDeclarativePrivate::s_runtimePlatform.isEmpty()) {
        const QString env = QString::fromLocal8Bit(getenv("PLASMA_PLATFORM"));
        KDeclarativePrivate::s_runtimePlatform = QStringList(env.split(QLatin1Char(':'), QString::SkipEmptyParts));
        if (KDeclarativePrivate::s_runtimePlatform.isEmpty()) {
            KConfigGroup cg(KSharedConfig::openConfig(), "General");
            KDeclarativePrivate::s_runtimePlatform = cg.readEntry(QStringLiteral("runtimePlatform"), KDeclarativePrivate::s_runtimePlatform);
        }
    }

    return KDeclarativePrivate::s_runtimePlatform;
}

void KDeclarative::setRuntimePlatform(const QStringList &platform)
{
    KDeclarativePrivate::s_runtimePlatform = platform;
}

}
