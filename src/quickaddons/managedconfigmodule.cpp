/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2019 Kevin Ottens <kevin.ottens@enioka.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managedconfigmodule.h"

#include <QPointer>

#include <KConfigCore/KCoreConfigSkeleton>

namespace KQuickAddons
{
class ManagedConfigModulePrivate
{
public:
    ManagedConfigModulePrivate(ManagedConfigModule *module)
        : _q(module)
    {
        QMetaObject::invokeMethod(_q, "_k_registerSettings", Qt::QueuedConnection);
    }

    void _k_registerSettings();

    ManagedConfigModule *_q;
    QList<QPointer<KCoreConfigSkeleton>> _skeletons;
};

#if QUICKADDONS_BUILD_DEPRECATED_SINCE(5, 88)
ManagedConfigModule::ManagedConfigModule(const KAboutData *aboutData, QObject *parent, const QVariantList &args)
    : ConfigModule(aboutData, parent, args)
    , d(new ManagedConfigModulePrivate(this))
{
}
#endif

#if QUICKADDONS_BUILD_DEPRECATED_SINCE(5, 88)
ManagedConfigModule::ManagedConfigModule(const KPluginMetaData &metaData, QObject *parent, const QVariantList &args)
    : ConfigModule(metaData, parent, args)
    , d(new ManagedConfigModulePrivate(this))
{
}
#endif

ManagedConfigModule::ManagedConfigModule(QObject *parent, const QVariantList &args)
    : ConfigModule(parent, args)
    , d(new ManagedConfigModulePrivate(this))
{
}

ManagedConfigModule::ManagedConfigModule(QObject *parent, const KPluginMetaData &metaData, const QVariantList &args)
    : ConfigModule(parent, metaData, args)
    , d(new ManagedConfigModulePrivate(this))
{
}

ManagedConfigModule::~ManagedConfigModule()
{
    delete d;
}

void ManagedConfigModule::load()
{
    for (const auto &skeleton : std::as_const(d->_skeletons)) {
        if (skeleton) {
            skeleton->load();
        }
    }
}

void ManagedConfigModule::save()
{
    for (const auto &skeleton : std::as_const(d->_skeletons)) {
        if (skeleton) {
            skeleton->save();
        }
    }
}

void ManagedConfigModule::defaults()
{
    for (const auto &skeleton : std::as_const(d->_skeletons)) {
        if (skeleton) {
            skeleton->setDefaults();
        }
    }
}

bool ManagedConfigModule::isSaveNeeded() const
{
    return false;
}

bool ManagedConfigModule::isDefaults() const
{
    return true;
}

void ManagedConfigModulePrivate::_k_registerSettings()
{
    const auto skeletons = _q->findChildren<KCoreConfigSkeleton *>();
    for (auto *skeleton : skeletons) {
        _q->registerSettings(skeleton);
    }
}

void ManagedConfigModule::settingsChanged()
{
    bool needsSave = false;
    bool representsDefaults = true;
    for (const auto &skeleton : std::as_const(d->_skeletons)) {
        if (skeleton) {
            needsSave |= skeleton->isSaveNeeded();
            representsDefaults &= skeleton->isDefaults();
        }
    }

    if (!needsSave) {
        needsSave = isSaveNeeded();
    }

    if (representsDefaults) {
        representsDefaults = isDefaults();
    }

    setRepresentsDefaults(representsDefaults);
    setNeedsSave(needsSave);
}

void ManagedConfigModule::registerSettings(KCoreConfigSkeleton *skeleton)
{
    if (!skeleton || d->_skeletons.contains(skeleton)) {
        return;
    }

    d->_skeletons.append(skeleton);

    auto settingsChangedSlotIndex = metaObject()->indexOfMethod("settingsChanged()");
    auto settingsChangedSlot = metaObject()->method(settingsChangedSlotIndex);

    QObject::connect(skeleton, &KCoreConfigSkeleton::configChanged, this, &ManagedConfigModule::settingsChanged);

    const auto items = skeleton->items();
    for (auto item : items) {
        const auto itemHasSignals = dynamic_cast<KConfigCompilerSignallingItem *>(item) || dynamic_cast<KPropertySkeletonItem *>(item);
        if (!itemHasSignals) {
            continue;
        }

        auto name = item->name();
        if (name.at(0).isUpper()) {
            name[0] = name[0].toLower();
        }

        const auto metaObject = skeleton->metaObject();
        const auto propertyIndex = metaObject->indexOfProperty(name.toUtf8().constData());
        const auto property = metaObject->property(propertyIndex);
        if (!property.hasNotifySignal()) {
            continue;
        }

        const auto changedSignal = property.notifySignal();
        QObject::connect(skeleton, changedSignal, this, settingsChangedSlot);
    }

    auto toRemove = std::remove_if(d->_skeletons.begin(), d->_skeletons.end(), [](const QPointer<KCoreConfigSkeleton> &value) {
        return value.isNull();
    });
    d->_skeletons.erase(toRemove, d->_skeletons.end());

    QMetaObject::invokeMethod(this, "settingsChanged", Qt::QueuedConnection);
}

}

#include "moc_managedconfigmodule.cpp"
