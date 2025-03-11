/*
    SPDX-FileCopyrightText: 2010 BetterInbox <contact@betterinbox.com>
    SPDX-FileContributor: Gregory Schlomoff <greg@betterinbox.com>

    SPDX-License-Identifier: MIT
*/

#include "DeclarativeMimeData.h"

DeclarativeMimeData::DeclarativeMimeData()
    : QMimeData()
    , m_source(nullptr)
{
}

DeclarativeMimeData::DeclarativeMimeData(const QMimeData *copy)
    : QMimeData()
    , m_source(nullptr)
{
    // Copy the standard MIME data
    const auto formats = copy->formats();
    for (const QString &format : formats) {
        QMimeData::setData(format, copy->data(format));
    }

    // If the object we are copying actually is a DeclarativeMimeData, copy our extended properties as well
    const DeclarativeMimeData *declarativeMimeData = qobject_cast<const DeclarativeMimeData *>(copy);
    if (declarativeMimeData) {
        this->setSource(declarativeMimeData->source());
    }
}

QUrl DeclarativeMimeData::url() const
{
    if (this->hasUrls() && !this->urls().isEmpty()) {
        return QMimeData::urls().constFirst();
    }
    return QUrl();
}
void DeclarativeMimeData::setUrl(const QUrl &url)
{
    if (this->url() == url) {
        return;
    }

    QList<QUrl> urlList;
    urlList.append(url);
    QMimeData::setUrls(urlList);
    Q_EMIT urlChanged();
}

QJsonArray DeclarativeMimeData::urls() const
{
    QJsonArray varUrls;
    const auto lstUrls = QMimeData::urls();
    for (const QUrl &url : lstUrls) {
        varUrls.append(url.toString());
    }
    return varUrls;
}

void DeclarativeMimeData::setUrls(const QJsonArray &urls)
{
    QList<QUrl> urlList;
    urlList.reserve(urls.size());
    for (const auto &varUrl : urls) {
        urlList << QUrl(varUrl.toString());
    }
    QMimeData::setUrls(urlList);
    Q_EMIT urlsChanged();
}

// color
QColor DeclarativeMimeData::color() const
{
    if (this->hasColor()) {
        return qvariant_cast<QColor>(this->colorData());
    }
    return QColor();
}

bool DeclarativeMimeData::hasColor() const
{
    // qDebug() << " hasColor " << (QMimeData::hasColor() ? color().name() : "false");
    return QMimeData::hasColor();
}

void DeclarativeMimeData::setColor(const QColor &color)
{
    if (this->color() != color) {
        this->setColorData(color);
        Q_EMIT colorChanged();
    }
}

void DeclarativeMimeData::setData(const QString &mimeType, const QVariant &data)
{
    if (data.userType() == QMetaType::QByteArray) {
        QMimeData::setData(mimeType, data.toByteArray());
    } else if (data.canConvert<QString>()) {
        QMimeData::setData(mimeType, data.toString().toLatin1());
    }
}

QQuickItem *DeclarativeMimeData::source() const
{
    return m_source;
}
void DeclarativeMimeData::setSource(QQuickItem *source)
{
    if (m_source != source) {
        m_source = source;
        Q_EMIT sourceChanged();
    }
}

QByteArray DeclarativeMimeData::getDataAsByteArray(const QString &format)
{
    return data(format);
}

#include "moc_DeclarativeMimeData.cpp"
