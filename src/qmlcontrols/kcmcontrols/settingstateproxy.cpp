/*
 *    Copyright 2020 Kevin Ottens <kevin.ottens@enioka.com>
 *    Copyright 2020 Cyril Rossi <cyril.rossi@enioka.com>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Library General Public
 *    License as published by the Free Software Foundation; either
 *    version 2 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Library General Public License for more details.
 *
 *    You should have received a copy of the GNU Library General Public License
 *    along with this library; see the file COPYING.LIB.  If not, write to
 *    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301, USA.
 */

#include "settingstateproxy.h"

#include <QDebug>
#include <QMetaMethod>

KCoreConfigSkeleton *SettingStateProxy::configObject() const
{
    return m_configObject;
}

void SettingStateProxy::setConfigObject(KCoreConfigSkeleton *configObject)
{
    if (m_configObject == configObject) {
        return;
    }

    if (m_configObject) {
        m_configObject->disconnect(this);
    }

    m_configObject = configObject;
    emit configObjectChanged();
    updateState();
    connectSetting();
}

QString SettingStateProxy::settingName() const
{
    return m_settingName;
}

void SettingStateProxy::setSettingName(const QString &settingName)
{
    if (m_settingName == settingName) {
        return;
    }

    if (m_configObject) {
        m_configObject->disconnect(this);
    }

    m_settingName = settingName;
    emit settingNameChanged();
    updateState();
    connectSetting();
}

bool SettingStateProxy::isImmutable() const
{
    return m_immutable;
}

bool SettingStateProxy::isDefaulted() const
{
    return m_defaulted;
}

void SettingStateProxy::updateState()
{
    const auto item = m_configObject ? m_configObject->findItem(m_settingName) : nullptr;
    const auto immutable = item ? item->isImmutable() : false;
    const auto defaulted = item ? item->isDefault() : true;

    if (m_immutable != immutable) {
        m_immutable = immutable;
        emit immutableChanged();
    }

    if (m_defaulted != defaulted) {
        m_defaulted = defaulted;
        emit defaultedChanged();
    }
}

void SettingStateProxy::connectSetting()
{
    const auto item = m_configObject ? m_configObject->findItem(m_settingName) : nullptr;
    if (!item) {
        return;
    }

    const auto updateStateSlotIndex = metaObject()->indexOfMethod("updateState()");
    Q_ASSERT(updateStateSlotIndex >= 0);
    const auto updateStateSlot = metaObject()->method(updateStateSlotIndex);
    Q_ASSERT(updateStateSlot.isValid());

    const auto itemHasSignals = dynamic_cast<KConfigCompilerSignallingItem*>(item) || dynamic_cast<KPropertySkeletonItem*>(item);
    if (!itemHasSignals) {
        qWarning() << "Attempting to use SettingStateProxy with a non signalling item:" << m_settingName;
        return;
    }

    const auto propertyName = [this] {
        auto name = m_settingName;
        if (name.at(0).isUpper()) {
            name[0] = name[0].toLower();
        }
        return name.toUtf8();
    }();

    const auto metaObject = m_configObject->metaObject();
    const auto propertyIndex = metaObject->indexOfProperty(propertyName.constData());
    Q_ASSERT(propertyIndex >= 0);
    const auto property = metaObject->property(propertyIndex);
    Q_ASSERT(property.isValid());
    if (!property.hasNotifySignal()) {
        qWarning() << "Attempting to use SettingStateProxy with a non notifying property:" << propertyName;
        return;
    }

    const auto changedSignal = property.notifySignal();
    Q_ASSERT(changedSignal.isValid());
    connect(m_configObject, changedSignal, this, updateStateSlot);
    connect(m_configObject, &KCoreConfigSkeleton::configChanged,
            this, &SettingStateProxy::updateState);
}
