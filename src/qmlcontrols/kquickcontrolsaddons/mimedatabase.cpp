/*
 *    Copyright 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
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

#include "mimedatabase.h"
#include <QJsonObject>
#include <QDebug>

static QJsonObject mimetypeToJsonObject(const QMimeType& type)
{
    if (!type.isValid()) {
        qWarning() << "trying to export an invalid type";
        return QJsonObject();
    }
    QJsonObject ret;
    ret["name"] = type.name();
    ret["iconName"] = type.iconName();
    ret["comment"] = type.comment();
    return ret;
}

MimeDatabase::MimeDatabase(QObject* parent)
    : QObject(parent)
{
}

QJsonObject MimeDatabase::mimeTypeForUrl(const QUrl& url) const
{
    return mimetypeToJsonObject(m_db.mimeTypeForUrl(url));
}

QJsonObject MimeDatabase::mimeTypeForName(const QString& name) const
{
    QMimeType type = m_db.mimeTypeForName(name);
    if (!type.isValid()) {
        qWarning() << "wrong mime name" << name;
        return QJsonObject();
    }
    return mimetypeToJsonObject(type);
}
