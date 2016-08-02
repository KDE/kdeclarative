/*
 *   Copyright 2013 Marco Martin <notmart@gmail.com>
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

#include "configpropertymap.h"

#include <QDebug>
#include <QJSValue>

#include <kcoreconfigskeleton.h>

namespace KDeclarative {

class ConfigPropertyMapPrivate {
public:
    ConfigPropertyMapPrivate(ConfigPropertyMap *map)
        : q(map)
    {
    }

    void loadConfig();
    void writeConfig();
    void writeConfigValue(const QString &key, const QVariant &value);

    ConfigPropertyMap *q;
    QPointer<KCoreConfigSkeleton> config;
};

ConfigPropertyMap::ConfigPropertyMap(KCoreConfigSkeleton *config, QObject *parent)
    : QQmlPropertyMap(this, parent),
      d(new ConfigPropertyMapPrivate(this))
{
    d->config = config;

    //FIXME: find a prettier way to connect without lambdas
    connect(config, &KCoreConfigSkeleton::configChanged,
            [=](){d->loadConfig();});
    connect(this, &ConfigPropertyMap::valueChanged,
            [=](const QString &key, const QVariant &value){d->writeConfigValue(key, value);});

    d->loadConfig();
}

ConfigPropertyMap::~ConfigPropertyMap()
{
    d->writeConfig();
    delete d;
}

QVariant ConfigPropertyMap::updateValue(const QString &key, const QVariant &input)
{
    Q_UNUSED(key);
    if (input.userType() == qMetaTypeId<QJSValue>()) {
        return input.value<QJSValue>().toVariant();
    }
    return input;
}

bool ConfigPropertyMap::isImmutable(const QString &key) const
{
    KConfigSkeletonItem *item = d->config.data()->findItem(key);
    if (item) {
        return item->isImmutable();
    }

    return false;
}

void ConfigPropertyMapPrivate::loadConfig()
{
    if (!config) {
        return;
    }

    foreach (KConfigSkeletonItem *item, config.data()->items()) {
        q->insert(item->key(), item->property());
        emit q->valueChanged(item->key(), item->property());
    }
}

void ConfigPropertyMapPrivate::writeConfig()
{
    if (!config) {
        return;
    }

    foreach (KConfigSkeletonItem *item, config.data()->items()) {
        item->setProperty(q->value(item->key()));
    }

    config.data()->blockSignals(true);
    config.data()->save();
    config.data()->blockSignals(false);
}

void ConfigPropertyMapPrivate::writeConfigValue(const QString &key, const QVariant &value)
{
    KConfigSkeletonItem *item = config.data()->findItem(key);
    if (item) {
        item->setProperty(value);
        config.data()->blockSignals(true);
        config.data()->save();
        //why read? read will update KConfigSkeletonItem::mLoadedValue,
        //allowing a write operation to be performed next time
        config.data()->read();
        config.data()->blockSignals(false);
    }
}

}

#include "moc_configpropertymap.cpp"
