/*
    SPDX-FileCopyrightText: 2016 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
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
