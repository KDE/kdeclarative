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

#include <kcoreconfigskeleton.h>

namespace KDeclarative {

class ConfigPropertyMapPrivate {
public:
    

    QPointer<KCoreConfigSkeleton> config;
};

ConfigPropertyMap::ConfigPropertyMap(KCoreConfigSkeleton *config, QObject *parent)
    : QQmlPropertyMap(parent),
      d(new ConfigPropertyMapPrivate)
{
    d->config = config;
    connect(config, &KCoreConfigSkeleton::configChanged,
            this, &ConfigPropertyMap::loadConfig);
    connect(this, &ConfigPropertyMap::valueChanged,
            this, &ConfigPropertyMap::writeConfigValue);

    loadConfig();
}

ConfigPropertyMap::~ConfigPropertyMap()
{
    writeConfig();
    delete d;
}

void ConfigPropertyMap::loadConfig()
{
    if (!d->config) {
        return;
    }

    foreach (KConfigSkeletonItem *item, d->config.data()->items()) {
        insert(item->key(), item->property());
    }
}

void ConfigPropertyMap::writeConfig()
{
    if (!d->config) {
        return;
    }

    foreach (KConfigSkeletonItem *item, d->config.data()->items()) {
        item->setProperty(value(item->key()));
    }

    d->config.data()->blockSignals(true);
    d->config.data()->writeConfig();
    d->config.data()->blockSignals(false);
}

void ConfigPropertyMap::writeConfigValue(const QString &key, const QVariant &value)
{
    KConfigSkeletonItem *item = d->config.data()->findItem(key);
    if (item) {
        item->setProperty(value);
        d->config.data()->blockSignals(true);
        d->config.data()->writeConfig();
        d->config.data()->blockSignals(false);
    }
}

}

#include "moc_configpropertymap.cpp"

