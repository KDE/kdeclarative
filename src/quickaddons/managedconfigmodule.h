/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2019 Kevin Ottens <kevin.ottens@enioka.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MANAGEDCONFIGMODULE_H
#define MANAGEDCONFIGMODULE_H

#include <KQuickAddons/ConfigModule>

class KCoreConfigSkeleton;

namespace KQuickAddons
{
class ManagedConfigModulePrivate;

/**
 * @class KQuickAddons::ManagedConfigModule managedconfigmodule.h KQuickAddons/ManagedConfigModule
 *
 * The base class for configuration modules using KConfigXT settings.
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
 * K_PLUGIN_CLASS_WITH_JSON(MyConfigModule, "myconfigmodule.json")
 * \endcode
 *
 * The constructor of the ManagedConfigModule then looks like this:
 * \code
 * MyConfigModule::MyConfigModule(QObject* parent, const KPluginMetaData &metaData, const QVariantList &args)
 *   : ManagedConfigModule(parent, metaData, args)
 * {
 *   .
 *   .
 *   .
 * }
 * \endcode
 *
 * The QML part must be in the KPackage format, installed under share/kpackage/kcms.
 * @see KPackage::Package
 *
 * The package must have the same name as the C++ plugin, to be installed
 * by CMake with the command:
 * \code
 * kpackage_install_package(packagedir kcm_componentName kcms)
 * \endcode
 * The "packagedir" is the subdirectory in the source tree where the package sources are
 * located, and "kcm_componentName" is the name of the C++ plugin. Finally "kcms" is the literal string "kcms",
 * so that the package is
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
 * See https://develop.kde.org/docs/extend/kcm/
 * for more detailed documentation.
 *
 * @since 5.65
 */
class QUICKADDONS_EXPORT ManagedConfigModule : public ConfigModule
{
    Q_OBJECT
public:
    /**
     * Base class for all KControlModules.
     *
     * @note do not emit changed signals here, since they are not yet connected
     *       to any slot.
     * @since 5.88
     */
    explicit ManagedConfigModule(QObject *parent, const KPluginMetaData &metaData, const QVariantList &args = QVariantList());

    /**
     * Destroys the module.
     */
    ~ManagedConfigModule() override;

public Q_SLOTS:
    /**
     * Load the configuration data into the module.
     *
     * This method is invoked whenever the module should read its configuration
     * (most of the times from a config file) and update the user interface.
     * This happens when the user clicks the "Reset" button in the control
     * center, to undo all of his changes and restore the currently valid
     * settings. It is also called right after construction.
     *
     * By default this will load the settings from the child setting objects
     * of this module.
     */
    void load() override;

    /**
     * Save the configuration data.
     *
     * The save method stores the config information as shown
     * in the user interface in the config files.
     * It is called when the user clicks "Apply" or "Ok".
     *
     * By default this will save the child setting objects
     * of this module.
     */
    void save() override;

    /**
     * Sets the configuration to sensible default values.
     *
     * This method is called when the user clicks the "Default"
     * button. It should set the display to useful values.
     *
     * By default this will reset to defaults the child setting objects
     * of this module.
     */
    void defaults() override;

protected Q_SLOTS:
    /**
     * Forces the module to reevaluate the saveNeeded and
     * representsDefault state.
     *
     * This is required for some modules which might have
     * some settings managed outside of KConfigXT objects.
     */
    void settingsChanged();

    /**
     * Allow to register manually settings class generated from a kcfg file.
     * Used by derived class when automatic discovery is not possible.
     * After skeleton is registered it will automatically call settingsChanged().
     *
     * @since 5.67
     */
    void registerSettings(KCoreConfigSkeleton *skeleton);

private:
    /**
     * Allows to indicate if the module requires saving.
     *
     * By default this returns false, it needs to be overridden only
     * if the module has state outside of the settings declared in
     * the KConfigXT classes it uses.
     */
    virtual bool isSaveNeeded() const;

    /**
     * Allows to indicate if the module state is representing its defaults.
     *
     * By default this returns true, it needs to be overridden only
     * if the module has state outside of the settings declared in
     * the KConfigXT classes it uses.
     */
    virtual bool isDefaults() const;

    Q_PRIVATE_SLOT(d, void _k_registerSettings())
    ManagedConfigModulePrivate *const d;
    friend class ManagedConfigModulePrivate;
};

}

#endif // MANAGEDCONFIGMODULE_H
