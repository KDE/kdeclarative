/*
    Copyright (C) 2010 by BetterInbox <contact@betterinbox.com>
    Original author: Gregory Schlomoff <greg@betterinbox.com>

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

#include "DeclarativeMimeData.h"

/*!
    \qmlclass MimeData DeclarativeMimeData

    This is a wrapper class around QMimeData, with a few extensions to provide better support for in-qml drag & drops.
*/

DeclarativeMimeData::DeclarativeMimeData()
    : QMimeData(),
    m_source(nullptr)
{}

/*!
    \internal
    \class DeclarativeMimeData

    Creates a new DeclarativeMimeData by cloning the QMimeData passed as parameter.
    This is useful for two reasons :
        - In DragArea, we want to clone our "working copy" of the DeclarativeMimeData instance, as Qt will automatically
        delete it after the drag and drop operation.
        - In the drop events, the QMimeData is const, and we have troubles passing const to QML. So we clone it to
        remove the "constness"

    This method will try to cast the QMimeData to DeclarativeMimeData, and will clone our extensions to QMimeData as well
*/
DeclarativeMimeData::DeclarativeMimeData(const QMimeData* copy)
    : QMimeData(),
    m_source(nullptr)
{
    // Copy the standard MIME data
    const auto formats = copy->formats();
    for (const QString &format : formats) {
        QMimeData::setData(format, copy->data(format));
    }

    // If the object we are copying actually is a DeclarativeMimeData, copy our extended properties as well
    const DeclarativeMimeData* declarativeMimeData = qobject_cast<const DeclarativeMimeData*>(copy);
    if (declarativeMimeData) {
        this->setSource(declarativeMimeData->source());
    }
}

/*!
    \qmlproperty url MimeData::url

    Returns the first URL from the urls property of QMimeData
    TODO: We should use QDeclarativeListProperty<QUrls> to return the whole list instead of only the first element.
*/
QUrl DeclarativeMimeData::url() const
{
    if ( this->hasUrls() && !this->urls().isEmpty()) {
        return QMimeData::urls().first();
     }
    return QUrl();
}
void DeclarativeMimeData::setUrl(const QUrl &url)
{
    if (this->url() == url)
        return;

    QList<QUrl> urlList;
    urlList.append(url);
    QMimeData::setUrls(urlList);
    emit urlChanged();
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
    for (const QVariant &varUrl : urls) {
        urlList << varUrl.toUrl();
    }
    QMimeData::setUrls(urlList);
    emit urlsChanged();
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
    //qDebug() << " hasColor " << (QMimeData::hasColor() ? color().name() : "false");
    return QMimeData::hasColor();
}

void DeclarativeMimeData::setColor(const QColor &color)
{
    if (this->color() != color) {
        this->setColorData(color);
        emit colorChanged();
    }
}

void DeclarativeMimeData::setData(const QString &mimeType, const QVariant &data)
{
    if (data.type() == QVariant::ByteArray) {
        QMimeData::setData(mimeType, data.toByteArray());
    } else if (data.canConvert(QVariant::String)) {
        QMimeData::setData(mimeType, data.toString().toLatin1());
    }
}

/*!
  \qmlproperty item MimeData::source

  Setting source to any existing qml item will enable the receiver of the drag and drop operation to know in which item
  the operation originated.

  In the case of inter-application drag and drop operations, the source will not be available, and will be 0.
  Be sure to test it in your QML code, before using it, or it will generate errors in the console.
*/
QQuickItem* DeclarativeMimeData::source() const
{
    return m_source;
}
void DeclarativeMimeData::setSource(QQuickItem* source)
{
    if (m_source != source) {
        m_source = source;
        emit sourceChanged();
    }
}

QByteArray DeclarativeMimeData::getDataAsByteArray(const QString& format)
{
    return data(format);
}
