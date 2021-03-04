/*
    SPDX-FileCopyrightText: 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kcmshell.h"

#include <KAuthorized>
#include <KService>

#include <KIO/CommandLauncherJob>

KCMShell::KCMShell(QObject *parent)
    : QObject(parent)
{
}

KCMShell::~KCMShell()
{
}

void KCMShell::open(const QStringList &names) const
{
    KIO::CommandLauncherJob *job = new KIO::CommandLauncherJob(QStringLiteral("kcmshell5"), names);
    job->start();
}

void KCMShell::openSystemSettings(const QString &name, const QStringList &args) const
{
    const QString systemSettings = QStringLiteral("systemsettings");
    KIO::CommandLauncherJob *job = nullptr;

    QStringList cmdline{name};
    if (!args.isEmpty()) {
        cmdline.append(QStringLiteral("--args"));
        cmdline.append(args.join(QLatin1Char(' ')));
    }

    // Open in System Settings if it's available'
    if (KService::serviceByDesktopName(systemSettings)) {
        job = new KIO::CommandLauncherJob(QStringLiteral("systemsettings5"), cmdline);
        job->setDesktopName(systemSettings);
    } else {
        job = new KIO::CommandLauncherJob(QStringLiteral("kcmshell5"), cmdline);
    }

    job->start();
}

void KCMShell::openInfoCenter(const QString &name) const
{
    const QString systemSettings = QStringLiteral("systemsettings");
    KIO::CommandLauncherJob *job = nullptr;

    // Open in Info Center if System Settings is available
    if (KService::serviceByDesktopName(systemSettings)) {
        job = new KIO::CommandLauncherJob(QStringLiteral("kinfocenter"), QStringList(name));
        job->setIcon(systemSettings);
        job->setDesktopName(systemSettings);
    } else {
        job = new KIO::CommandLauncherJob(QStringLiteral("kcmshell5"), QStringList(name));
    }

    job->start();
}

QStringList KCMShell::authorize(const QStringList &menuIds) const
{
    return KAuthorized::authorizeControlModules(menuIds);
}
