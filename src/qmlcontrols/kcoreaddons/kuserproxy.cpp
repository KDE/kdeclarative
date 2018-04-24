/*
Copyright 2013 Marco Martin <mart@kde.org>
Copyright 2014 Sebastian Kügler <sebas@kde.org>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) version 3, or any
later version accepted by the membership of KDE e.V. (or its
successor approved by the membership of KDE e.V.), which shall
act as a proxy defined in Section 6 of version 3 of the license.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "kuserproxy.h"
#include <QFile>
#include <QDir>
#include <QHostInfo>
#include <QTextStream>
#include <QUrl>

#include <KDirWatch>

#include <QDebug>

const QString etcPasswd = QStringLiteral("/etc/passwd");
const QString accountsServiceIconPath = QStringLiteral("/var/lib/AccountsService/icons");

KUserProxy::KUserProxy (QObject *parent)
    : QObject(parent),
    m_temporaryEmptyFaceIconPath(false)
{
    QString pathToFaceIcon(m_user.faceIconPath());
    if (pathToFaceIcon.isEmpty()) {
        //KUser returns null if the current faceIconPath is empty
        //so we should explicitly watch ~/.face.icon rather than faceIconPath()
        //as we want to watch for this file being created
        pathToFaceIcon = QDir::homePath() + QStringLiteral("/.face.icon");
    }

    m_dirWatch.addFile(pathToFaceIcon);
    m_dirWatch.addFile(accountsServiceIconPath + QLatin1Char('/') + m_user.loginName());
    if (QFile::exists(etcPasswd)) {
        m_dirWatch.addFile(etcPasswd);
    }

    connect(&m_dirWatch, &KDirWatch::dirty, this, &KUserProxy::update);
    connect(&m_dirWatch, &KDirWatch::created, this, &KUserProxy::update);
    connect(&m_dirWatch, &KDirWatch::deleted, this, &KUserProxy::update);
}

KUserProxy::~KUserProxy()
{
}

void KUserProxy::update(const QString &path)
{
    if (path == m_user.faceIconPath() || path == QDir::homePath() + QLatin1String("/.face.icon")
            || path == accountsServiceIconPath + QLatin1Char('/') + m_user.loginName()) {
        // we need to force updates, even when the path doesn't change,
        // but the underlying image does. Change path temporarily, to
        // make the Image reload.
        // Needs cache: false in the Image item to actually reload
        m_temporaryEmptyFaceIconPath = true;
        emit faceIconUrlChanged();
        m_temporaryEmptyFaceIconPath = false;
        emit faceIconUrlChanged();
    } else if (path == etcPasswd) {
        m_user = KUser();
        emit nameChanged();
    }
}

QString KUserProxy::fullName() const
{
    QString fullName = m_user.property(KUser::FullName).toString();
    if (!fullName.isEmpty()) {
        return fullName;
    }
    return loginName();
}

QString KUserProxy::loginName() const
{
    return m_user.loginName();
}

QUrl KUserProxy::faceIconUrl() const
{
    if (m_temporaryEmptyFaceIconPath) {
        return QUrl();
    }
    const QString u = m_user.faceIconPath();
    const QFile f(u);
    if (f.exists(u)) {
        // We need to return a file URL, not a simple path
        return QUrl::fromLocalFile(u);
    }
    return QUrl();
}

QString KUserProxy::os()
{
    if (m_os.isEmpty()) {
        QFile osfile(QStringLiteral("/etc/os-release"));
        if (osfile.exists()) {
            if (!osfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return QString();
            }

            QTextStream in(&osfile);
            while(!in.atEnd()) {
                QString line = in.readLine();
                if (line.startsWith(QLatin1String("PRETTY_NAME"))) {
                    QStringList fields = line.split(QLatin1String("PRETTY_NAME=\""));
                    if (fields.count() == 2) {
                        osfile.close();
                        QString pretty = fields.at(1);
                        pretty.chop(1);
                        m_os = pretty;
                        return pretty;
                    }
                }
            }
        }
    }
    return m_os;
}

QString KUserProxy::host() const
{
    return QHostInfo::localHostName();
}

