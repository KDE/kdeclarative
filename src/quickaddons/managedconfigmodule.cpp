/*
   This file is part of the KDE libraries

   Copyright (C) 2019 Kevin Ottens <kevin.ottens@enioka.com>

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

#include "managedconfigmodule.h"

#include <KConfigCore/KCoreConfigSkeleton>

namespace KQuickAddons {

class ManagedConfigModulePrivate
{
public:
    ManagedConfigModulePrivate(ManagedConfigModule *module):
        _q(module)
    {
        QMetaObject::invokeMethod(_q, "_k_registerSettings", Qt::QueuedConnection);
    }

    void _k_registerSettings();

    ManagedConfigModule *_q;
    QList<KCoreConfigSkeleton*> _skeletons;
};

ManagedConfigModule::ManagedConfigModule(const KAboutData *aboutData, QObject *parent, const QVariantList &args)
    : ConfigModule(aboutData, parent, args),
      d(new ManagedConfigModulePrivate(this))
{
}

ManagedConfigModule::ManagedConfigModule(const KPluginMetaData &metaData, QObject *parent, const QVariantList &args)
    : ConfigModule(metaData, parent, args),
      d(new ManagedConfigModulePrivate(this))
{
}

ManagedConfigModule::ManagedConfigModule(QObject *parent, const QVariantList &args)
    : ConfigModule(parent, args),
      d(new ManagedConfigModulePrivate(this))
{
}

ManagedConfigModule::~ManagedConfigModule()
{
    delete d;
}

void ManagedConfigModule::load()
{
    for (const auto skeleton : qAsConst(d->_skeletons)) {
        skeleton->load();
    }
}

void ManagedConfigModule::save()
{
    for (const auto skeleton : qAsConst(d->_skeletons)) {
        skeleton->save();
    }
}

void ManagedConfigModule::defaults()
{
    for (const auto skeleton : qAsConst(d->_skeletons)) {
        skeleton->setDefaults();
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
    auto settingsChangedSlotIndex = _q->metaObject()->indexOfMethod("settingsChanged()");
    auto settingsChangedSlot = _q->metaObject()->method(settingsChangedSlotIndex);

    _skeletons = _q->findChildren<KCoreConfigSkeleton*>();
    for (auto skeleton : qAsConst(_skeletons)) {
        QObject::connect(skeleton, &KCoreConfigSkeleton::configChanged, _q, &ManagedConfigModule::settingsChanged);

        const auto items = skeleton->items();
        for (auto item : items) {
            const auto itemHasSignals = dynamic_cast<KConfigCompilerSignallingItem*>(item) || dynamic_cast<KPropertySkeletonItem*>(item);

            if (!itemHasSignals) {
                continue;
            }

            auto name = item->name();
            if (name.at(0).isUpper())
                name[0] = name[0].toLower();

            const auto metaObject = skeleton->metaObject();
            const auto propertyIndex = metaObject->indexOfProperty(name.toUtf8().constData());
            const auto property = metaObject->property(propertyIndex);
            if (!property.hasNotifySignal()) {
                continue;
            }

            const auto changedSignal = property.notifySignal();
            QObject::connect(skeleton, changedSignal, _q, settingsChangedSlot);
        }
    }

    _q->settingsChanged();
}

void ManagedConfigModule::settingsChanged()
{
    bool needsSave = false;
    bool representsDefaults = true;
    for (const auto skeleton : qAsConst(d->_skeletons)) {
        needsSave |= skeleton->isSaveNeeded();
        representsDefaults &= skeleton->isDefaults();
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

}

#include "moc_managedconfigmodule.cpp"

