/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kdeclarative.h"
#include "private/kdeclarative_p.h"
#include "private/kioaccessmanagerfactory_p.h"
#include "qmlobject.h"

#include <QCoreApplication>
#include <QQmlContext>
#include <QQmlDebuggingEnabler>

#include <KConfigGroup>
#include <KQuickIconProvider>
#include <KSharedConfig>

namespace KDeclarative
{
QStringList KDeclarativePrivate::s_runtimePlatform;

KDeclarativePrivate::KDeclarativePrivate()
    : contextObj(nullptr)
{
}

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 100)
KDeclarative::KDeclarative()
    : d(new KDeclarativePrivate)
{
}
#endif

KDeclarative::~KDeclarative()
{
    if (d->declarativeEngine) {
        delete d->declarativeEngine->networkAccessManagerFactory();
        d->declarativeEngine->setNetworkAccessManagerFactory(nullptr);
    }
    delete d;
}

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 100)
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
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 0)
void KDeclarative::initialize()
{
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 45)
void KDeclarative::setupBindings()
{
    setupContext();
    setupEngine(d->declarativeEngine.data());
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 75)
void KDeclarative::setupContext()
{
    /*Create a context object for the root qml context.
      in this way we can register global functions, in this case the i18n() family*/
    if (!d->contextObj) {
        d->contextObj = new KLocalizedContext(d->declarativeEngine.data());
    }

    // If the engine is in a qmlObject take the qmlObject rootContext instead of the engine one.
    if (d->qmlObj) {
        d->qmlObj->rootContext()->setContextObject(d->contextObj);
    } else {
        d->declarativeEngine.data()->rootContext()->setContextObject(d->contextObj);
    }

    if (!d->translationDomain.isNull()) {
        d->contextObj->setTranslationDomain(d->translationDomain);
    }
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 98)
void KDeclarative::setupEngine(QQmlEngine *engine)
{
#ifndef Q_OS_ANDROID
    // get rid of stock network access manager factory
    QQmlNetworkAccessManagerFactory *factory = engine->networkAccessManagerFactory();
    engine->setNetworkAccessManagerFactory(nullptr);
    delete factory;
    engine->setNetworkAccessManagerFactory(new KIOAccessManagerFactory());
#endif

    /* Tell the engine to search for platform-specific imports first
       (so it will "win" in import name resolution).
       addImportPath adds the path at the beginning, so to honour user's
       paths we need to traverse the list in reverse order */
    const QStringList pluginPathList = engine->importPathList();

    const QString target = componentsTarget();
    if (target != defaultComponentsTarget()) {
        const QStringList paths = pluginPathList;
        QStringListIterator it(paths);
        it.toBack();
        while (it.hasPrevious()) {
            QString path = it.previous();
            path = path.left(path.lastIndexOf(QLatin1Char('/')));
            engine->addImportPath(path + QStringLiteral("/platformqml/") + target);
        }
    }

    // setup ImageProvider for KDE icons
    engine->addImageProvider(QStringLiteral("icon"), new KQuickIconProvider);
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 75)
void KDeclarative::setTranslationDomain(const QString &translationDomain)
{
    d->translationDomain = translationDomain;
    if (d->contextObj) {
        d->contextObj->setTranslationDomain(d->translationDomain);
    }
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 75)
QString KDeclarative::translationDomain() const
{
    return d->translationDomain;
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 98)
void KDeclarative::setupQmlJsDebugger()
{
#if QT_CONFIG(qml_debug)
    if (QCoreApplication::arguments().contains(QLatin1String("-qmljsdebugger"))) {
        QQmlDebuggingEnabler enabler;
    }
#endif
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
        KDeclarativePrivate::s_runtimePlatform = QStringList(env.split(QLatin1Char(':'), Qt::SkipEmptyParts));
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
#endif
}
