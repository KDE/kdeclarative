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

#include "kcmshell.h"

#include <QProcess>

#include <KAuthorized>

KCMShell::KCMShell(QObject *parent) : QObject(parent)
{

}

KCMShell::~KCMShell()
{

}

void KCMShell::open(const QStringList &names) const
{
    QProcess::startDetached(QStringLiteral("kcmshell5"), names);
}

QStringList KCMShell::authorize(const QStringList &menuIds) const
{
    return KAuthorized::authorizeControlModules(menuIds);
}
