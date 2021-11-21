/*
    SPDX-FileCopyrightText: 2014 Antonis Tsiapaliokas <antonis.tsiapaliokas@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "krunproxy.h"

#include <QMimeDatabase>

#include <KIO/ApplicationLauncherJob>
#include <KIO/JobUiDelegate>
#include <KIO/OpenUrlJob>
#include <KNotificationJobUiDelegate>
#include <KService>

KRunProxy::KRunProxy(QObject *parent)
    : QObject(parent)
{
    qWarning() << "Using KRun from QML is deprecated (since 5.88)."
               << "Use Qt.openUrlExternally, or the KIO::OpenUrlJob/KIO::ApplicationLauncherJob classes from C++ instead.";
}

bool KRunProxy::openUrl(const QString &file)
{
    QUrl fileUrl(file);
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForUrl(fileUrl);
    const QString fileMimeType = mime.name();

    if (fileMimeType == QLatin1String("application/x-executable") || !mime.isValid()) {
        // for security reasons we should not be able to execute applications.
        // We should use its desktop file to access it.
        return false;
    }

    if (fileMimeType == QLatin1String("application/x-desktop") && fileUrl.isLocalFile()) {
        // If our mimetype is a desktop file, then we don't want to open
        // the desktop file itself but the application in which it is associated
        // with.
        return openService(fileUrl.toLocalFile());
    } else {
        KIO::OpenUrlJob *job = new KIO::OpenUrlJob(fileUrl, fileMimeType);
        // JobUiDelegate is widgets-based, but that's currently the only way to get the open-with dialog
        job->setUiDelegate(new KIO::JobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, nullptr));
        job->start();
        return true;
    }
}

bool KRunProxy::openService(const QString &serviceName)
{
    KService::Ptr service = KService::serviceByDesktopName(serviceName);
    if (service) {
        KIO::ApplicationLauncherJob *job = new KIO::ApplicationLauncherJob(service);
        job->setUiDelegate(new KNotificationJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled));
        job->start();
        return true;
    }
    return false;
}
