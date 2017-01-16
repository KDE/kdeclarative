/***************************************************************************
 *   Copyright 2015 Kai Uwe Broulik <kde@privat.broulik.de>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef KCMSHELL_H
#define KCMSHELL_H

#include <QObject>

class KCMShell : public QObject
{
    Q_OBJECT

public:
    explicit KCMShell(QObject *parent = nullptr);
    virtual ~KCMShell();

public Q_SLOTS:
    void open(const QStringList &names) const;

    /**
     * Check which of the given control modules the user is
     * allowed to access
     *
     * This can be used for example to hide context menu options
     * that would do nothing if the user wasn't authorized.
     *
     * @code
     * visible: KCMShell.authorize(["org.kde.fooconfig.desktop"]).length > 0
     * @endcode
     *
     * @param menuIds A list of control module menu IDs
     * @return The entries in @p menuIds that the user is
     * authorized to access, may be empty
     * @sa KAuthorized::authorizeControlModules
     * @since 5.23
     */
    QStringList authorize(const QStringList &menuIds) const;

};

#endif // KCMSHELL_H
