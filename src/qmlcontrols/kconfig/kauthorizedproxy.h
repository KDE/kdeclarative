/*
 * * Copyright 2016 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include <QObject>

class KAuthorizedProxy : public QObject
{
    Q_OBJECT

public:
    explicit KAuthorizedProxy(QObject *parent = nullptr);
    ~KAuthorizedProxy() override;

    /**
     * Returns whether the user is permitted to perform a certain action.
     */
    Q_INVOKABLE bool authorize(const QString &action) const;

    /**
     * Returns whether the user is permitted to perform a certain action.
     * This behaves like authorize(), except that "action/" is prepended to action
     */
    Q_INVOKABLE bool authorizeAction(const QString &action) const;

};
