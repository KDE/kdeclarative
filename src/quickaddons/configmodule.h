/*
    This file is part of the KDE libraries

    SPDX-FileCopyrightText: 1999 Matthias Hoelzer-Kluepfel <hoelzer@kde.org>
    SPDX-FileCopyrightText: 2001 Michael Goffioul <kdeprint@swing.be>
    SPDX-FileCopyrightText: 2004 Frans Englich <frans.englich@telia.com>
    SPDX-FileCopyrightText: 2009 Dario Freddi <drf@kde.org>
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef CONFIGMODULE_H
#define CONFIGMODULE_H

#include "quickaddons_export.h"

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QtQml>

#include <KPluginMetaData>

class KAboutData;
class QQuickItem;
class QQmlEngine;

namespace KQuickAddons
{
class ConfigModulePrivate;

/**
 * @class KQuickAddons::ConfigModule configmodule.h KQuickAddons/ConfigModule
 *
 * The base class for configuration modules.
 *
 * Configuration modules are realized as plugins that are loaded only when
 * needed.
 *
 * The module in principle is a simple widget displaying the
 * item to be changed. The module has a very small interface.
 *
 * All the necessary glue logic and the GUI bells and whistles
 * are provided by the control center and must not concern
 * the module author.
 *
 * To write a config module, you have to create a C++ library
 * and an accompaning QML user interface.
 * The library must contain a factory function like the following:
 *
 * \code
 * #include <KPluginFactory>
 *
 * K_PLUGIN_FACTORY(MyConfigModuleFactory, registerPlugin<MyConfigModule>() )
 * \endcode
 *
 * The constructor of the ConfigModule then looks like this:
 * \code
 * YourConfigModule::YourConfigModule( QObject* parent )
 *   : ConfigModule( parent )
 * {
 *   KAboutData *about = new KAboutData(
 *     <kcm name>, i18n( "..." ),
 *     KDE_VERSION_STRING, QString(), KAboutLicense::GPL,
 *     i18n( "Copyright 2006 ..." ) );
 *   about->addAuthor( i18n(...) );
 *   setAboutData( about );
 *   .
 *   .
 *   .
 * }
 * \endcode
 *
 * The QML part must be in the KPackage format, installed under share/kpackage/kcms.
 * @see KPackage::Package
 * 
 * The package must have the same name as the KAboutData componentName, to be installed
 * by CMake with the command:
 * \code
 * kpackage_install_package(packagedir kcm_componentName kcms)
 * \endcode
 * The "packagedir" is the subdirectory in the source tree where the package sources are
 * located, and "kcm_componentName" is the componentname passed to the KAboutData in the
 * C++ part. Finally "kcms" is the literal string "kcms", so that the package is
 * installed as a configuration module (and not some other kind of package).
 * The main config dialog UI will be the file
 * ui/main.qml from the package (or what X-KPackage-MainScript value is in the
 * package metadata desktop file).
 *
 * The QML part can access all the properties of ConfigModule (together with the properties
 * defined in its subclass) by accessing to the global object "kcm", or with the
 * import of "org.kde.kcm 1.0" the ConfigModule attached property.
 *
 * \code
 * import QtQuick 2.1
 * import QtQuick.Controls 1.0 as QtControls
 * import org.kde.kcm 1.0
 * import org.kde.plasma.core 2.0 as PlasmaCore
 *
 * Item {
 *     //implicitWidth and implicitHeight will be used as initial size
 *     //when loaded in kcmshell5
 *     implicitWidth: units.gridUnit * 20
 *     implicitHeight: units.gridUnit * 20
 *
 *     ConfigModule.buttons: ConfigModule.Help|ConfigModule.Apply
 *     Label {
 *         text: kcm.needsSave
 *     }
 * }
 * \endcode
 *
 * See https://techbase.kde.org/Development/Tutorials/KCM_HowTo
 * for more detailed documentation.
 *
 */
class QUICKADDONS_EXPORT ConfigModule : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickItem *mainUi READ mainUi CONSTANT)
    Q_PROPERTY(KQuickAddons::ConfigModule::Buttons buttons READ buttons WRITE setButtons NOTIFY buttonsChanged)
    Q_PROPERTY(bool needsSave READ needsSave WRITE setNeedsSave NOTIFY needsSaveChanged)
    Q_PROPERTY(bool representsDefaults READ representsDefaults WRITE setRepresentsDefaults NOTIFY representsDefaultsChanged)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString quickHelp READ quickHelp WRITE setQuickHelp NOTIFY quickHelpChanged)
    Q_PROPERTY(QString rootOnlyMessage READ rootOnlyMessage WRITE setRootOnlyMessage NOTIFY rootOnlyMessageChanged)
    Q_PROPERTY(bool useRootOnlyMessage READ useRootOnlyMessage WRITE setUseRootOnlyMessage NOTIFY useRootOnlyMessageChanged)
    Q_PROPERTY(bool needsAuthorization READ needsAuthorization WRITE setNeedsAuthorization NOTIFY needsAuthorizationChanged)
    Q_PROPERTY(int columnWidth READ columnWidth WRITE setColumnWidth NOTIFY columnWidthChanged)
    Q_PROPERTY(int depth READ depth NOTIFY depthChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int defaultsIndicatorsVisible READ defaultsIndicatorsVisible WRITE setDefaultsIndicatorsVisible NOTIFY defaultsIndicatorsVisibleChanged)

public:
    /**
     * An enumeration type for the buttons used by this module.
     * You should only use Help, Default and Apply. The rest is obsolete.
     * NoAdditionalButton can be used when we do not want have other button that Ok Cancel
     *
     * @see ConfigModule::buttons @see ConfigModule::setButtons
     */
    enum Button {
        NoAdditionalButton = 0,
        Help = 1,
        Default = 2,
        Apply = 4,
    };
    Q_ENUM(Button)
    Q_DECLARE_FLAGS(Buttons, Button)
    Q_FLAG(Buttons)

#if QUICKADDONS_ENABLE_DEPRECATED_SINCE(5, 88)
    /**
     * Base class for all KControlModules.
     *
     * @note do not emit changed signals here, since they are not yet connected
     *       to any slot.
     * @param aboutData becomes owned by the ConfigModule
     * @deprecated since 5.88, use ConfigModule(QObject*, KPluginMetaData, QVariantList)
     */
    QUICKADDONS_DEPRECATED_VERSION(5, 88, "Use ConfigModule(QObject*, KPluginMetaData, QVariantList)")
    explicit ConfigModule(const KAboutData *aboutData, QObject *parent = nullptr, const QVariantList &args = QVariantList());
#endif

#if QUICKADDONS_ENABLE_DEPRECATED_SINCE(5, 88)
    /**
     * @note do not emit changed signals here, since they are not yet connected
     *       to any slot.
     * @param metaData description for the plugin: it will generate a KAboutData from that
     * @since 5.11
     * @deprecated since 5.88, use ConfigModule(QObject*, KPluginMetaData, QVariantList)
     */
    QUICKADDONS_DEPRECATED_VERSION(5, 88, "Use ConfigModule(QObject*, KPluginMetaData, QVariantList)")
    explicit ConfigModule(const KPluginMetaData &metaData, QObject *parent = nullptr, const QVariantList &args = QVariantList());
#endif

    /**
     * Base class for all KControlModules.
     *
     * @note do not emit changed signals here, since they are not yet connected
     *       to any slot.
     * @since 5.88
     */
    explicit ConfigModule(QObject *parent, const KPluginMetaData &metaData, const QVariantList &args = QVariantList());

    /**
     * Base class for all KControlModules.
     *
     * @note do not emit changed signals here, since they are not yet connected
     *       to any slot.
     */
    explicit ConfigModule(QObject *parent = nullptr, const QVariantList &args = QVariantList());

    /**
     * Destroys the module.
     */
    ~ConfigModule() override;

#if QUICKADDONS_ENABLE_DEPRECATED_SINCE(5, 88)
    /**
     * This is generally only called for the KBugReport.
     * If you override you should  have it return a pointer to a constant.
     *
     *
     * @returns the KAboutData for this module
     * @deprecated since 5.88. Use the KPluginMetaData the ConfigModule was instantiated from.
     */
    QUICKADDONS_DEPRECATED_VERSION(5, 88, "Use the KPluginMetaData the ConfigModule was instantiated from")
    const KAboutData *aboutData() const;
#endif

    /**
     * This sets the KAboutData returned by aboutData()
     * The about data is now owned by ConfigModule.
     */
    void setAboutData(const KAboutData *about);

    /**
     * @brief Set if the module's save() method requires authorization to be executed
     *
     * It will still have to execute the action itself using the KAuth library, so
     * this method is not technically needed to perform the action, but
     * using this method will ensure that hosting
     * applications like System Settings or kcmshell behave correctly.
     *
     * @param action the action that will be used by this ConfigModule
     */
    void setAuthActionName(const QString &action);

    /**
     * Returns the action previously set with setAuthActionName(). By default its an invalid action.
     *
     * @return The action that has to be authorized to execute the save() method.
     */
    QString authActionName() const;

    /**
     * @return the qml engine that built the main config UI
     */
    QQmlEngine *engine() const;

    /**
     * The status of the mainUi component.
     * @since 5.64
     */
    QQmlComponent::Status status() const;

    /**
     * The error string in case the mainUi failed to load.
     * @return 5.64
     */
    QString errorString() const;

    // QML property accessors

    /**
     * @return The main UI for this configuration module. It's a QQuickItem coming from
     * the QML package named the same as the KAboutData's component name for
     * this config module
     */
    QQuickItem *mainUi();

    /*
     * @return a subpage at a given depth
     * @note This does not include the mainUi. i.e a depth of 2 is a mainUi and one subPage
     * at index 0
     */
    QQuickItem *subPage(int index) const;

    /**
     * Sets the quick help.
     */
    void setQuickHelp(const QString &help);

    /**
     * Return a quick-help text.
     *
     * This method is called when the module is docked.
     * The quick-help text should contain a short description of the module and
     * links to the module's help files. You can use HTML formatting tags in the text.
     *
     * @note make sure the quick help text gets translated (use i18n()).
     */
    QString quickHelp() const;

    /**
     * Set this property to true when the user changes something in the module,
     * signaling that a save (such as user pressing Ok or Apply) is needed.
     */
    void setNeedsSave(bool needs);

    /**
     * True when the module has something changed and needs save.
     */
    bool needsSave();

    /**
     * Set this property to true when the user sets the state of the module
     * to the default settings (e.g. clicking Defaults would do nothing).
     */
    void setRepresentsDefaults(bool defaults);

    /**
     * True when the module state represents the default settings.
     */
    bool representsDefaults();

    /**
     * Sets the buttons to display.
     *
     * Help: shows a "Help" button.
     *
     * Default: shows a "Use Defaults" button.
     *
     * Apply: in kcontrol this will show an "Apply" and "Reset" button,
     *        in kcmshell this will show an "Ok", "Apply" and "Cancel" button.
     *
     * If Apply is not specified, kcmshell will show a "Close" button.
     *
     * @see ConfigModule::buttons
     */
    void setButtons(const Buttons btn);

    /**
     * Indicate which buttons will be used.
     *
     * The return value is a value or'ed together from
     * the Button enumeration type.
     *
     * @see ConfigModule::setButtons
     */
    Buttons buttons() const;

    /**
     * Sets the RootOnly message.
     *
     * This message will be shown at the top of the module if useRootOnlyMessage is
     * set. If no message is set, a default one will be used.
     *
     * @see ConfigModule::rootOnlyMessage
     */
    void setRootOnlyMessage(const QString &message);

    /**
     * Get the RootOnly message for this module.
     *
     * When the module must be run as root, or acts differently
     * for root and a normal user, it is sometimes useful to
     * customize the message that appears at the top of the module
     * when used as a normal user. This function returns this
     * customized message. If none has been set, a default message
     * will be used.
     *
     * @see ConfigModule::setRootOnlyMessage
     */
    QString rootOnlyMessage() const;

    /**
     * Change whether or not the RootOnly message should be shown.
     *
     * Following the value of @p on, the RootOnly message will be
     * shown or not.
     *
     * @see ConfigModule::useRootOnlyMessage
     */
    void setUseRootOnlyMessage(bool on);

    /**
     * Tell if KControl should show a RootOnly message when run as
     * a normal user.
     *
     * In some cases, the module don't want a RootOnly message to
     * appear (for example if it has already one). This function
     * tells KControl if a RootOnly message should be shown
     *
     * @see ConfigModule::setUseRootOnlyMessage
     */
    bool useRootOnlyMessage() const;

    /**
     * @brief Set if the module's save() method requires authorization to be executed.
     *
     * The module can set this property to @c true if it requires authorization.
     * It will still have to execute the action itself using the KAuth library, so
     * this method is not technically needed to perform the action, but
     * using this and/or the setAuthActionName() method will ensure that hosting
     * applications like System Settings or kcmshell behave correctly.
     *
     * Called with @c true, this method will set the action to  "org.kde.kcontrol.name.save" where
     * "name" is aboutData()->appName() return value. This default action won't be set if
     * the aboutData() object is not valid.
     *
     * Note that called with @c false, this method will reset the action name set with setAuthActionName().
     *
     * @param needsAuth Tells if the module's save() method requires authorization to be executed.
     */
    void setNeedsAuthorization(bool needsAuth);

    /**
     * Returns the value previously set with setNeedsAuthorization() or setAuthActionName(). By default it's @c false.
     *
     * @return @c true if the module's save() method requires authorization, @c false otherwise
     */
    bool needsAuthorization() const;

    /**
     * @returns the name of the config module
     * @since 5.41
     */
    QString name() const;

    /**
     * @returns the description of the config module
     * @since 5.41
     */
    QString description() const;

    /**
     * returns the width the kcm wants in column mode.
     * If a columnWidth is valid ( > 0 ) and less than the systemsettings' view width,
     * more than one will be visible at once, and the first page will be a sidebar to the last page pushed.
     * As default, this is -1 which will make the shell always show only one page at a time.
     * @since 5.50
     */
    int columnWidth() const;

    /**
     * Sets the column width we want.
     * @since 5.50
     */
    void setColumnWidth(int width);

    /**
     * @returns how many pages this kcm has.
     * It is guaranteed to be at least 1 (the main ui) plus how many times a new page has been pushed without pop
     */
    int depth() const;

    /**
     * Sets the current page index this kcm should display
     * @since 5.53
     */
    void setCurrentIndex(int index);

    /**
     * @returns the index of the page this kcm should display
     * @since 5.53
     */
    int currentIndex() const;

    static ConfigModule *qmlAttachedProperties(QObject *object);

    /**
     * @returns defaultness indicator visibility
     * @since 5.73
     */
    bool defaultsIndicatorsVisible() const;

public Q_SLOTS:
    /**
     * Load the configuration data into the module.
     *
     * The load method sets the user interface elements of the
     * module to reflect the current settings stored in the
     * configuration files.
     *
     * This method is invoked whenever the module should read its configuration
     * (most of the times from a config file) and update the user interface.
     * This happens when the user clicks the "Reset" button in the control
     * center, to undo all of his changes and restore the currently valid
     * settings. It is also called right after construction.
     */
    virtual void load();

    /**
     * Save the configuration data.
     *
     * The save method stores the config information as shown
     * in the user interface in the config files.
     *
     * If necessary, this method also updates the running system,
     * e.g. by restarting applications. This normally does not apply for
     * KSettings::Dialog modules where the updating is taken care of by
     * KSettings::Dispatcher.
     *
     * save is called when the user clicks "Apply" or "Ok".
     *
     */
    virtual void save();

    /**
     * Sets the configuration to sensible default values.
     *
     * This method is called when the user clicks the "Default"
     * button. It should set the display to useful values.
     */
    virtual void defaults();

    /**
     * Push a new sub page in the KCM hierarchy: pages will be seen as a Kirigami PageRow
     * @since 5.50
     */
    void push(const QString &fileName, const QVariantMap &propertyMap = QVariantMap());

    /**
     *
     */
    void push(QQuickItem *item);

    /**
     * pop the last page of the KCM hierarchy, the page is destroyed
     * @since 5.50
     */
    void pop();

    /**
     * remove and return the last page of the KCM hierarchy:
     * the popped page won't be deleted, it's the caller's responsibility to manage the lifetime of the returned item
     * @returns the last page if any, nullptr otherwise
     * @since 5.89
     */
    QQuickItem *takeLast();

    /**
     * Ask the shell to show a passive notification
     * @param message The message text to display
     * @param timeout (optional) the timeout, either in milliseconds or the strings "short" and "long"
     * @param actionText (optional) The notification can have a button with this text
     * @param callBack (optional) If actionText is set and callBack is a JavaScript function, it will be called when the use clicks the button.
     * @since 5.68
     */
    void showPassiveNotification(const QString &message,
                                 const QVariant &timeout = QVariant(),
                                 const QString &actionText = QString(),
                                 const QJSValue &callBack = QJSValue());

    /**
     * Change defaultness indicator visibility
     * @param visible
     * @since 5.73
     */
    void setDefaultsIndicatorsVisible(bool visible);

Q_SIGNALS:

    /**
     * The auth action name has changed: this signal will relay it to the actual KCM
     */
    void authActionNameChanged();

    // QML NOTIFY signaling
    /**
     * Buttons to display changed.
     */
    void buttonsChanged();

    /**
     * Indicate that the module's quickhelp has changed.
     *
     * Emit this signal whenever the module's quickhelp changes.
     * Modules implemented as tabbed dialogs might want to implement
     * per-tab quickhelp for example.
     *
     */
    void quickHelpChanged();

    /**
     * Indicate that the state of the modules contents has changed.
     *
     * This signal is emitted whenever the state of the configuration
     * shown in the module changes. It allows the module container to
     * keep track of unsaved changes.
     */
    void needsSaveChanged();

    /**
     * Indicate that the state of the modules contents has changed
     * in a way that it might represents the defaults settings, or
     * stopped representing them.
     */
    void representsDefaultsChanged();

    /**
     * Emits this signal whenever the need for root authorization to save changes.
     */
    void needsAuthorizationChanged();

    /**
     * Indicate that the module's root message has changed.
     *
     * Emits this signal whenever the module's root message changes.
     *
     */
    void rootOnlyMessageChanged();

    /**
     * Emits this signal whenever the root only message gets used or discarded.
     */
    void useRootOnlyMessageChanged();

    /**
     * Emitted when a new sub page is pushed
     * @since 5.50
     */
    void pagePushed(QQuickItem *page);

    /**
     * Emitted when a sub page is popped
     * @since 5.50
     */
    // RFC: page argument?
    void pageRemoved();

    /**
     * Emitted when the wanted column width of the kcm changes
     * @since 5.50
     */
    void columnWidthChanged(int width);

    /**
     * Emitted when the current page changed
     * @since 5.53
     */
    void currentIndexChanged(int index);

    /**
     * Emitted when the number of pages changed
     * @since 5.53
     */
    void depthChanged(int index);

    /**
     * Emitted when the kcm wants the shell to display a passive notification
     * @since 5.68
     */
    void passiveNotificationRequested(const QString &message, const QVariant &timeout, const QString &actionText, const QJSValue &callBack);

    /**
     * Emitted when kcm need to display indicators for field with non default value
     * @since 5.73
     */
    void defaultsIndicatorsVisibleChanged();

private:
    ConfigModulePrivate *const d;
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(KQuickAddons::ConfigModule::Buttons)
QML_DECLARE_TYPEINFO(KQuickAddons::ConfigModule, QML_HAS_ATTACHED_PROPERTIES)

#endif // ConfigModule_H
