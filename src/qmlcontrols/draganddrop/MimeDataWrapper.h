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

#ifndef MIMEDATAWRAPPER_H
#define MIMEDATAWRAPPER_H

#include <QObject>
#include <QString>
#include <QJsonArray>

class QMimeData;
class QObject;
/**
 * Exposes a const QMimeData instance
 *
 * In contrast to DeclarativeMimeData, doesn't create a copy of the QMimeData instance
 */
class MimeDataWrapper : public QObject
{
    Q_OBJECT

    /**
     * A plain text (MIME type text/plain) representation of the data.
     */
    Q_PROPERTY(QString text READ text CONSTANT)

    /**
     * A string if the data stored in the object is HTML (MIME type text/html); otherwise returns an empty string.
     */
    Q_PROPERTY(QString html READ html CONSTANT)

    /**
     * Url contained in the mimedata
     */
    Q_PROPERTY(QUrl url READ url CONSTANT)

    /**
     * A list of URLs contained within the MIME data object.
     * URLs correspond to the MIME type text/uri-list.
     */
    Q_PROPERTY(QJsonArray urls READ urls CONSTANT)

    /**
     * A color if the data stored in the object represents a color (MIME type application/x-color); otherwise QVariant().
     */
    Q_PROPERTY(QVariant color READ color CONSTANT)

    /**
     * The graphical item on the scene that started the drag event. It may be null.
     */
    Q_PROPERTY(QVariant source READ source CONSTANT)

    /**
     * Mimetypes provided by the mime data instance
     *
     * @sa QMimeData::formats
     */
    Q_PROPERTY(QStringList formats READ formats CONSTANT)

    /**
     * @sa QMimeData::hasUrls
     */
    Q_PROPERTY(bool hasUrls READ hasUrls CONSTANT)

    /**
     * @returns the wrapped object
     */
    Q_PROPERTY(QMimeData* mimeData READ mimeData CONSTANT)

public:
    MimeDataWrapper(const QMimeData* data, QObject* parent);

    QString text() const;
    QString html() const;
    QUrl url() const;
    QJsonArray urls() const;
    bool hasUrls() const;
    QVariant color() const;
    QStringList formats() const;
    QVariant source() const;
    QMimeData* mimeData() const;

    Q_INVOKABLE QByteArray getDataAsByteArray(const QString& format);

private:
    const QMimeData* m_data;
};

#endif
