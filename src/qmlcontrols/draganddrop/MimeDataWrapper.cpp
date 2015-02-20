/*
    Copyright (C) 2015 by Aleix Pol Gonzalez <aleixpol@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "MimeDataWrapper.h"
#include <QMimeData>
#include <QUrl>
#include <QColor>

MimeDataWrapper::MimeDataWrapper(const QMimeData* data, QObject* parent)
    : QObject(parent)
    , m_data(data)
{}

QString MimeDataWrapper::text() const
{
    return m_data->text();
}

QString MimeDataWrapper::html() const
{
    return m_data->html();
}

QUrl MimeDataWrapper::url() const
{
    if ( m_data->hasUrls() && !m_data->urls().isEmpty()) {
        return m_data->urls().first();
    }
    return QUrl();
}

bool MimeDataWrapper::hasUrls() const
{
    return m_data->hasUrls();
}

QJsonArray MimeDataWrapper::urls() const
{
    QJsonArray varUrls;
    foreach (const QUrl &url, m_data->urls()) {
        varUrls.append(url.toString());
    }
    return varUrls;
}

QVariant MimeDataWrapper::color() const
{
    if (m_data->hasColor())
        return m_data->colorData();
    else
        return QVariant();
}

QStringList MimeDataWrapper::formats() const
{
    return m_data->formats();
}


QByteArray MimeDataWrapper::getDataAsByteArray(const QString& format)
{
    return m_data->data(format);
}

QVariant MimeDataWrapper::source() const
{
//     In case it comes from a DeclarativeMimeData
    return m_data->property("source");
}

QMimeData* MimeDataWrapper::mimeData() const
{
    return const_cast<QMimeData*>(m_data);
}
