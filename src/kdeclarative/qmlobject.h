/*
    SPDX-FileCopyrightText: 2013 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef QMLOBJECT_H
#define QMLOBJECT_H

#include <QObject>

#include <QQmlComponent>

#include <kdeclarative/kdeclarative_export.h>

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 98)
#include <KPackage/Package>
#endif

class QQmlEngine;
class QQmlComponent;
class QQmlContext;

namespace KDeclarative
{
class QmlObjectPrivate;

/**
 * @class KDeclarative::QmlObject qmlobject.h KDeclarative/QmlObject
 *
 * @author Marco Martin <mart@kde.org>
 *
 * @short An object that instantiates an entire QML context, with its own declarative engine
 *
 * KDeclarative::QmlObject provides a class to conveniently use QML based
 * declarative user interfaces inside Plasma widgets.
 * A QmlObject corresponds to one QML file (which can include others).
 * It will have its own QQmlEngine with a single root object,
 * described in the QML file.
 */
class KDECLARATIVE_EXPORT QmlObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl source READ source WRITE setSource)
    Q_PROPERTY(QString translationDomain READ translationDomain WRITE setTranslationDomain)
    Q_PROPERTY(bool initializationDelayed READ isInitializationDelayed WRITE setInitializationDelayed)
    Q_PROPERTY(QObject *rootObject READ rootObject)
    Q_PROPERTY(QQmlComponent::Status status READ status NOTIFY statusChanged)

public:
    /**
     * Constructs a new QmlObject
     *
     * @param parent the parent of this object
     */
    explicit QmlObject(QObject *parent = nullptr);

    /**
     * Constructs a new QmlObject
     *
     * @param engine a QQmlEngine we want to use
     * @param parent the parent of this object
     *
     * @deprecated Since 5.95, Use QmlObject(std::shared_ptr<QmlEngine>, QObject*)
     * instead.
     */
#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 95)
    KDECLARATIVE_DEPRECATED_VERSION(5, 95, "Use QmlObject(std::shared_ptr<QQmlEngine>, QmlContext*, QObject*) instead")
    explicit QmlObject(QQmlEngine *engine, QObject *parent = nullptr);
#endif

    /**
     * Constructs a new QmlObject
     *
     * @param engine the QQmlEngine to use
     * @param rootContext the root context to use for object creation
     * @param parent the parent of this object
     *
     * @deprecated Since 5.95, Use QmlObject(std::shared_ptr<QmlEngine>, QmlContext*, QObject*)
     * instead.
     */
#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 95)
    KDECLARATIVE_DEPRECATED_VERSION(5, 95, "Use QmlObject(std::shared_ptr<QQmlEngine>, QQmlContext*, QObject*) instead")
    explicit QmlObject(QQmlEngine *engine, QQmlContext *rootContext, QObject *parent = nullptr);
#endif

    /**
     * Construct a new QmlObject
     *
     * @param engine The QQmlEngine to use. If this object is the first user of
     * the engine (e.g. use_count() is 1), KDeclarative::setupEngine() will be
     * called. If this is nullptr, a new engine will be created for this object
     * to use.
     * @param rootContext The QML context to use for object creation. If this is
     * nullptr, the engine's root context will be used.
     * @param parent The QObject parent for this object.
     */
    explicit QmlObject(std::shared_ptr<QQmlEngine> engine, QQmlContext *rootContext = nullptr, QObject *parent = nullptr);

    ~QmlObject() override;

    /**
     * Call this method before calling setupBindings to install a translation domain for all
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
     * Sets the path of the QML file to parse and execute
     *
     * @param path the absolute path of a QML file
     */
    void setSource(const QUrl &source);

    /**
     * @return the absolute path of the current QML file
     */
    QUrl source() const;

#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 98)
    /**
     * Load the package called packageName, then loads the
     * mainscript file for that package
     *
     * @param packageName the plugin name of the package
     * @seprecated Since 5.98, use KPackage manually and set the source URL to the "mainscript" file path
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "use KPackage manually and set the source URL to the \"mainscript\" file path")
    void loadPackage(const QString &packageName);

    /**
     * Sets a package, then loads the
     * mainscript file for that package
     *
     * @param package the package we want to use to provide QML
     *         files to this QML object
     * @seprecated Since 5.98, use KPackage manually and set the source URL to the "mainscript" file path
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "use KPackage manually and set the source URL to the \"mainscript\" file path")
    void setPackage(const KPackage::Package &package);

    /**
     * @return the optional package, if any
     * @seprecated Since 5.98, use KPackage manually and set the source URL to the "mainscript" file path
     */
    KDECLARATIVE_DEPRECATED_VERSION(5, 98, "use KPackage manually and set the source URL to the \"mainscript\" file path")
    KPackage::Package package() const;
#endif

    /**
     * Sets whether the execution of the QML file has to be delayed later in the event loop. It has to be called before setQmlPath().
     * In this case it will be possible to assign new objects in the main engine context
     * before the main component gets initialized.
     * In that case it will be possible to access it immediately from the QML code.
     * The initialization will either be completed automatically asynchronously
     * or explicitly by calling completeInitialization()
     *
     * @param delay if true the initialization of the QML file will be delayed
     *              at the end of the event loop
     */
    void setInitializationDelayed(const bool delay);

    /**
     * @return true if the initialization of the QML file will be delayed
     *              at the end of the event loop
     */
    bool isInitializationDelayed() const;

    /**
     * @return the declarative engine that runs the qml file assigned to this widget.
     */
    QQmlEngine *engine();

    /**
     * @return the root object of the declarative object tree
     */
    QObject *rootObject() const;

    /**
     * @return the main QQmlComponent of the engine
     */
    QQmlComponent *mainComponent() const;

    /**
     * The components's creation context.
     * @since 5.11
     */
    QQmlContext *rootContext() const;

    /**
     * The component's current status.
     * @since 5.11
     */
    QQmlComponent::Status status() const;

    /**
     * Creates and returns an object based on the provided url to a Qml file
     * with the same QQmlEngine and the same root context as the main object,
     * that will be the parent of the newly created object
     * @param source url where the QML file is located
     * @param context The QQmlContext in which we will create the object,
     *             if 0 it will use the engine's root context
     * @param initialProperties optional properties that will be set on
     *             the object when created (and before Component.onCompleted
     *             gets emitted
     */
    QObject *createObjectFromSource(const QUrl &source, QQmlContext *context = nullptr, const QVariantHash &initialProperties = QVariantHash());

    /**
     * Creates and returns an object based on the provided QQmlComponent
     * with the same QQmlEngine and the same root context as the admin object,
     * that will be the parent of the newly created object
     * @param component the component we want to instantiate
     * @param context The QQmlContext in which we will create the object,
     *             if 0 it will use the engine's root context
     * @param initialProperties optional properties that will be set on
     *             the object when created (and before Component.onCompleted
     *             gets emitted
     */
    QObject *createObjectFromComponent(QQmlComponent *component, QQmlContext *context = nullptr, const QVariantHash &initialProperties = QVariantHash());

public Q_SLOTS:
    /**
     * Finishes the process of initialization.
     * If isInitializationDelayed() is false, calling this will have no effect.
     * @param initialProperties optional properties that will be set on
     *             the object when created (and before Component.onCompleted
     *             gets emitted
     */
    void completeInitialization(const QVariantHash &initialProperties = QVariantHash());

Q_SIGNALS:
    /**
     * Emitted when the parsing and execution of the QML file is terminated
     */
    void finished();

    void statusChanged(QQmlComponent::Status);

protected:
    /**
     * Constructs a new QmlObject
     *
     * @param engine a QQmlEngine we want to use
     * @param rootContext the root context we want to use for objects creation
     * @param obj setupEngine is called when this is set to nullptr. This way the creator can
     * influence if the engine should be initialized or not in case it is shared between
     * multiple objects (such as QmlObjectSharedEngine)
     * @param parent the parent of this object
     * @since 5.45
     *
     * @deprecated Since 5.95, Use QmlObject(std::shared_ptr<QmlEngine>, QmlContext*, QObject*)
     * instead. The "obj" parameter has been dropped, instead setupEngine will be
     * called if this QmlObject is the first user of the engine.
     */
#if KDECLARATIVE_ENABLE_DEPRECATED_SINCE(5, 95)
    KDECLARATIVE_DEPRECATED_VERSION(5, 95, "Use QmlObject(std::shared_ptr<QQmlEngine>, QQmlContext*, QObject*) instead")
    explicit QmlObject(QQmlEngine *engine, QQmlContext *rootContext, QmlObject *obj, QObject *parent = nullptr);
#endif

private:
    friend class QmlObjectPrivate;
    QmlObjectPrivate *const d;

    Q_PRIVATE_SLOT(d, void scheduleExecutionEnd())
    Q_PRIVATE_SLOT(d, void checkInitializationCompleted())
};

}

#endif // multiple inclusion guard
