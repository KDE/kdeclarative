/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KDECLARATIVE_H
#define KDECLARATIVE_H

#include <QQmlEngine>

#include <kdeclarative/kdeclarative_export.h>

#include <QStringList>

class QQmlEngine;

namespace KDeclarative
{
class KDeclarativePrivate;

/**
 * @class KDeclarative::KDeclarative kdeclarative.h <KDeclarative/KDeclarative>
 *
 * The KDeclarative class is used to manipulate the QQmlEngine instance used by
 * the application and to get some information about the platform,
 * that influences the behavior of the QML components.
 *
 * In order to use it, you will need a pointer to a QQmlEngine, and call
 * `setupEngine(engine)` at least once on the engine.
 * @code
 *     KDeclarative::KDeclarative::setupEngine(engine);  // if not done elsewhere
 * @endcode
 *
 * To setup integration with KI18n's translation methods and thus being able to call
 * i18n() from anywhere in your QML code., you set a KLocalizedContext from Ki18n
 * directly like this (since KF 5.17):
 * @code
 *     KLocalizedContext *localizedContextObject = new KLocalizedContext(engine);
 *     // if not using the global application domain, set custom domain name
 *     localizedContextObject->setTranslationDomain(QStringLiteral("mydomainname"));
 *     engine->rootContext()->setContextObject(localizedContextObject);
 * @endcode
 *
 * In case your code should work with KF versions before 5.17, use these deprecated
 * calls instead:
 * @code
 *     KDeclarative::KDeclarative decl;
 *     decl.setDeclarativeEngine(engine);
 *     // if not using the global application domain, set custom domain name
 *     decl.setTranslationDomain(QStringLiteral("mydomainname"));
 *     decl.setupContext();
 * @endcode
 *
 * @deprecated since 5.100.
 *
 * To set up translations use KLocalizedContext instead.
 *
 * Set the KQuickIconProvider from KIconThemes and the QQmlNetworkAccessManagerFactory creating a KIO::Integration::AccessManager
 * manually up if needed
 *
 */
class KDECLARATIVE_EXPORT KDeclarative
{
public:
#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 100)
    KDECLARATIVE_DEPRECATED_VERSION(5, 100, "See class API docs")
    explicit KDeclarative();
#endif
    ~KDeclarative();

    KDeclarative(const KDeclarative &) = delete;
    KDeclarative &operator=(const KDeclarative &) = delete;

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 0)
    /**
     * @deprecated since 5.0. The method is a no-op now, any call can be simply removed.
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 0, "Is a no-op")
    void initialize();
#endif

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 45)
    /**
     * Call this after setDeclarativeEngine to set the i18n global functions, the runtime platform, etc
     *
     * @deprecated since 5.45 use setupContext() and setupEngine()
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 45, "Call setupContext() and setupEngine() independently")
    void setupBindings();
#endif

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 75)
    /**
     * Call this after setDeclarativeEngine to set the i18n global functions.
     *
     * @sa setupEngine
     * @since 5.45
     * @deprecated Since 5.17 set KLocalizedContext directly, see documentation in KI18n::KLocalizedContext
     */
    KDECLARATIVE_DEPRECATED_VERSION_BELATED(5, 75, 5, 17, "set KLocalizedContext directly, see documentation in KI18n::KLocalizedContext")
    void setupContext();
#endif

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 100)
    /**
     * Assign a specific QQmlEngine to be used in this KDeclarative.
     *
     * A KDeclarative object works with a specific QQmlEngine. There
     * is no default engine, so you **must** call this function with a
     * non-null pointer to an engine before calling setupBindings()
     * or setupContext(), which set properties on the engine.
     *
     * The KDeclarative object does not take ownership of the engine.
     *
     * @param engine the engine to use in this KDeclarative object
     * @sa setupContext(), setupEngine()
     * @since 5.0
     */
    void setDeclarativeEngine(QQmlEngine *engine);
#endif

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 100)
    /**
     * @return the engine assigned to this KDeclarative.
     *         The engine may be a @c nullptr . No ownership is transferred.
     * @sa setDeclarativeEngine(), setupEngine()
     * @since 5.0
     */
    QQmlEngine *declarativeEngine() const;
#endif

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 75)
    /**
     * Call this method before calling setupContext to install a translation domain for all
     * i18n global functions. If a translation domain is set all i18n calls delegate to the
     * matching i18nd calls with the provided translation domain.
     *
     * The translationDomain affects all i18n calls including those from imports. Because of
     * that modules intended to be used as imports should prefer the i18nd variants and set
     * the translation domain explicitly in each call.
     *
     * This method is only required if your declarative usage is inside a library. If it's
     * in an application there is no need to set the translation domain as the application's
     * domain can be used.
     *
     * @param translationDomain The translation domain to be used for i18n calls.
     * @since 5.0
     * @deprecated Since 5.17 use KLocalizedContext::setTranslationDomain
     */
    KDECLARATIVE_DEPRECATED_VERSION_BELATED(5, 75, 5, 17, "set via KLocalizedContext::setTranslationDomain")
    void setTranslationDomain(const QString &translationDomain);
#endif

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 75)
    /**
     * @return the translation domain for the i18n calls done in this QML engine
     * @since 5.0
     * @deprecated Since 5.17 use KLocalizedContext::translationDomain
     */
    KDECLARATIVE_DEPRECATED_VERSION_BELATED(5, 75, 5, 17, "available via KLocalizedContext::translationDomain")
    QString translationDomain() const;
#endif

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 98)
    /**
     * This method must be called very early at startup time to ensure the
     * QQuickDebugger is enabled. Ideally it should be called in main(),
     * after command-line options are defined.
     * @since 5.0
     * @deprecated Since 5.98, deprecated for lack of usage. Set the debugger manually up
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "Deprecated for lack of usage. Set the debugger manually up")
    static void setupQmlJsDebugger();

    /**
     * @return the runtime platform, e.g. "desktop" or "tablet, touch". The first entry/ies in
     *         the list relate to the platform formfactor and the last is the input method
     *         specialization. If the string is empty, there is no specified runtime platform
     *         and a traditional desktop environment may be assumed
     * @since 4.10
     * @deprecated Since 5.98, use KRuntimePlatform::runtimePlatform instead
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "Use KRuntimePlatform::runtimePlatform instead")
    static QStringList runtimePlatform();

    /**
     * Sets the runtime platform from now on, globally to the process.
     * Already loaded QML components won't be affected.
     * @since 5.0
     * @deprecated Since 5.98, deprecated for lack of usage, set PLASMA_PLATFORM ENV variable instead
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "Deprecated for lack of usage, set PLASMA_PLATFORM ENV variable instead")
    static void setRuntimePlatform(const QStringList &platform);

    /**
     * @return the QML components target, based on the runtime platform. e.g. touch or desktop
     * @since 4.10
     * @deprecated Since 5.98, deprecated for lack of usage, use last value from KRuntimePlatform::runtimePlatform instead
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "Deprecated for lack of usage, use last value from KRuntimePlatform::runtimePlatform instead")
    static QString componentsTarget();

    /**
     * @return the default components target; can be used to compare against the returned value
     *         from @see componentsTarget()
     * @since 4.10
     * @deprecated Since 5.98, deprecated for lack of usage, use "desktop" as hardcoded value instead
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "Deprecated for lack of usage, use \"desktop\" as hardcoded value instead")
    static QString defaultComponentsTarget();

    /**
     * Setup a QML engine for use with any KDeclarative object.
     *
     * This needs to be done only once per QQmlEngine instance. An
     * engine that is shared between KDeclarative objects only needs
     * to be setup once. The engine is setup for the component target
     * (runtime platform) that is configured at the time setupEngine()
     * is called. The following things are added to the engine:
     * - a KIOAccessManagerFactory, replacing any existing stock QQmlNetworkAccessManagerFactory
     * - a QML icon provider, enabling the Image {} element to load images using the scheme "image:/"
     *
     * @param engine the engine to setup
     * @sa setupContext(), componentsTarget()
     * @since 5.45
     * @deprecated Since 5.98, set the KQuickIconProvider from KIconThemes and the QQmlNetworkAccessManagerFactory creating a KIO::Integration::AccessManager
     * manually up if needed
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "See API docs")
    static void setupEngine(QQmlEngine *engine);
#endif

private:
    KDeclarativePrivate *const d;
    friend class QmlObject;
};

}

#endif
