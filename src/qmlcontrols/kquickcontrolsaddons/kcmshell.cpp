/*
 *    Copyright 2015 Kai Uwe Broulik <kde@privat.broulik.de>
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

#include "kcmshell.h"

#include <QProcess>

#include <KAuthorized>
#include <KService>

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

void KCMShell::openSystemSettings(const QString &name) const
{
    auto service = KService::serviceByDesktopName(QStringLiteral("systemsettings"));
    if (service) {
        QProcess::startDetached(QStringLiteral("systemsettings5"), QStringList(name));
    } else {
        QProcess::startDetached(QStringLiteral("kcmshell5"), QStringList(name));
    }
}

void KCMShell::openInfoCenter(const QString &name) const
{
    auto service = KService::serviceByDesktopName(QStringLiteral("systemsettings"));
    if (service) {
        QProcess::startDetached(QStringLiteral("kinfocenter"), QStringList(name));
    } else {
        QProcess::startDetached(QStringLiteral("kcmshell5"), QStringList(name));
    }
}

QStringList KCMShell::authorize(const QStringList &menuIds) const
{
    return KAuthorized::authorizeControlModules(menuIds);
}
