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

class KCoreConfigSkeleton;

#include <kdeclarative/kdeclarative_export.h>

namespace KDeclarative {

class ConfigPropertyMapPrivate;

/**
 * @class KDeclarative::ConfigPropertyMap configpropertymap.h KDeclarative/ConfigPropertyMap
 *
 * TODO
 */
class KDECLARATIVE_EXPORT ConfigPropertyMap : public QQmlPropertyMap
{
    Q_OBJECT

public:
    ConfigPropertyMap(KCoreConfigSkeleton *config, QObject *parent = nullptr);
    ~ConfigPropertyMap() override;

    /**
     * Indicates if the map calls save() on the config object on each write or not
     *
     * @return true if the map automatically saves (the default), false otherwise
     * @since 5.65
     */
    bool isAutosave() const;

    /**
     * Allows to set the map into autosave mode or not
     *
     * @param autosave the new value for autosaving
     * @since 5.65
     */
    void setAutosave(bool autosave);

    /**
     * @brief Whether the value at the given key is immutable
     *
     * @return true if the value is immutable, false if it isn't or it doesn't exist
     */
    Q_INVOKABLE bool isImmutable(const QString &key) const;

protected:
    QVariant updateValue(const QString &key, const QVariant &input) override;
private:
    ConfigPropertyMapPrivate *const d;

};

}

#endif
