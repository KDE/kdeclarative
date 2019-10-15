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
 * TODO
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
    KDECLARATIVE_DEPRECATED_VERSION(5, 45, "See API docs")
    void setupBindings();
#endif

    /**
     * Call this after setDeclarativeEngine to set the i18n global functions.
     *
     * @since 5.45
     * @sa setupEngine
     */
    void setupContext();

    void setDeclarativeEngine(QQmlEngine *engine);
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
     * Already loaded QML components won't be affected
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
     * Setup the QML engine used by this KDeclarative object.
     *
     * This needs to be done only once per QQmlEngine instance.
     *
     * @since 5.45
     * @param engine The engine to setup
     * @sa setupContext
     */
    static void setupEngine(QQmlEngine *engine);

private:
    KDeclarativePrivate *const d;
    friend class QmlObject;
};

}

#endif
