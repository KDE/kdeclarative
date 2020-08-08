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

namespace KDeclarative {

class KDeclarativePrivate;

/**
 * @class KDeclarative::KDeclarative kdeclarative.h <KDeclarative/KDeclarative>
 *
 * The KDeclarative class is used to manipulate the QQmlEngine instance used by 
 * the application and to get some informations about the platform, 
 * that influences the behavior of the QML components.
 * 
 * In order to use it, create an instance of KDeclarative. You will need a 
 * pointer to a QQmlEngine, and call **both** `setDeclarativeEngine(engine)` and
 * `setupContext()` on your instance. You need to call `setupEngine(engine)`
 * at least once on the engine as well.
 * 
 *     KDeclarative::setupEngine(engine);  // if not done elsewhere
 *     KDeclarative::KDeclarative decl;
 *     decl.setDeclarativeEngine(engine);
 *     decl.setupContext();
 *     
 * This will add the following things to the engine:
 * - Use a KIOAccessManagerFactory instead of the stock QQmlNetworkAccessManagerFactory
 * - Add a QML icon provider, that makes possible for the Image {} element to load images using the scheme "image:/"
 * - Use the given engine for this context.
 * - Set a new rootContextObject() that exposes all the i18n() functions from the KI18n framework.
 *   They will be available in the global QML context: just call i18n() from anywhere in your QML code.
 * 
 */
class KDECLARATIVE_EXPORT KDeclarative
{
public:
    explicit KDeclarative();
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

    /**
     * Call this after setDeclarativeEngine to set the i18n global functions.
     *
     * @sa setupEngine
     * @since 5.45
     */
    void setupContext();

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
    /**
     * @return the engine assigned to this KDeclarative.
     *         The engine may be a @c nullptr . No ownership is transferred.
     * @sa setDeclarativeEngine(), setupEngine()
     * @since 5.0
     */
    QQmlEngine *declarativeEngine() const;

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
     */
    void setTranslationDomain(const QString &translationDomain);

    /**
     * @return the translation domain for the i18n calls done in this QML engine
     * @since 5.0
     */
    QString translationDomain() const;

    /**
     * This method must be called very early at startup time to ensure the
     * QQuickDebugger is enabled. Ideally it should be called in main(),
     * after command-line options are defined.
     * @since 5.0
     */
    static void setupQmlJsDebugger();

    /**
     * @return the runtime platform, e.g. "desktop" or "tablet, touch". The first entry/ies in
     *         the list relate to the platform formfactor and the last is the input method
     *         specialization. If the string is empty, there is no specified runtime platform
     *         and a traditional desktop environment may be assumed
     * @since 4.10
     */
    static QStringList runtimePlatform();

    /**
     * Sets the runtime platform from now on, globally to the process.
     * Already loaded QML components won't be affected.
     * @since 5.0
     */
    static void setRuntimePlatform(const QStringList &platform);

    /**
     * @return the QML components target, based on the runtime platform. e.g. touch or desktop
     * @since 4.10
     */
    static QString componentsTarget();

    /**
     * @return the default components target; can be used to compare against the returned value
     *         from @see componentsTarget()
     * @since 4.10
     */
    static QString defaultComponentsTarget();

    /**
     * Setup a QML engine for use with any KDeclarative object.
     *
     * This needs to be done only once per QQmlEngine instance. An
     * engine that is shared between KDeclarative objects only needs
     * to be setup once. The engine is setup for the component target
     * (runtime platform) that is configured at the time setupEngine()
     * is called.
     *
     * @param engine the engine to setup
     * @sa setupContext(), componentsTarget()
     * @since 5.45
     */
    static void setupEngine(QQmlEngine *engine);

private:
    KDeclarativePrivate *const d;
    friend class QmlObject;
};

}

#endif
