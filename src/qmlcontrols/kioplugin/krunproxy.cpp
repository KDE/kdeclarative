/*
 * Copyright 2014 Antonis Tsiapaliokas <antonis.tsiapaliokas@kde.org>
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

#include "krunproxy.h"

#include <QMimeDatabase>

#include <KRun>
#include <KService>

KRunProxy::KRunProxy(QObject* parent)
    : QObject(parent)
{
}

bool KRunProxy::openUrl(const QString &file)
{
    QUrl fileUrl(file);
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(fileUrl.isLocalFile() ? fileUrl.toLocalFile() : fileUrl.path());
    const QString fileMimeType = mime.name();

    if (fileMimeType == QLatin1String("application/x-executable") || !mime.isValid()) {
        //for security reasons we should not be able to execute applications.
        //We should use its desktop file to access it.
        return false;
    }

    if (fileMimeType == QLatin1String("application/x-desktop") && fileUrl.isLocalFile()) {
        // If our mimetype is a desktop file, then we don't want to open
        // the desktop file itself but the application in which it is associated
        // with.
        return openService(fileUrl.toLocalFile());
    } else {
        return KRun::runUrl(fileUrl, fileMimeType, nullptr, KRun::RunFlags{});
    }
}

bool KRunProxy::openService(const QString &serviceName)
{
    KService::Ptr service = KService::serviceByDesktopName(serviceName);
    if(service)
        return KRun::runApplication(*service, QList<QUrl>(), nullptr) != 0;
    return false;
}
