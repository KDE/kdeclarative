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

#ifndef CONFIGPROPERTYMAP_P
#define CONFIGPROPERTYMAP_P

#include <QQmlPropertyMap>
#include <QPointer>

class KCoreConfigSkeleton;

#include <kdeclarative/kdeclarative_export.h>

namespace KDeclarative {

class ConfigPropertyMapPrivate;

class KDECLARATIVE_EXPORT ConfigPropertyMap : public QQmlPropertyMap
{
    Q_OBJECT

public:
    ConfigPropertyMap(KCoreConfigSkeleton *config, QObject *parent = 0);
    ~ConfigPropertyMap();

    /**
     * @brief Whether the value at the given key is immutable
     *
     * @return true if the value is immutable, false if it isn't or it doesn't exist
     */
    Q_INVOKABLE bool isImmutable(const QString &key) const;

protected:
    QVariant updateValue(const QString &key, const QVariant &input) Q_DECL_OVERRIDE;
private:
    ConfigPropertyMapPrivate *const d;

    Q_PRIVATE_SLOT(d, void loadConfig())
    Q_PRIVATE_SLOT(d, void writeConfigValue(const QString &key, const QVariant &value))
};

}

#endif
