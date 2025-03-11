/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef MIMEDATAWRAPPER_H
#define MIMEDATAWRAPPER_H

#include <QJsonArray>
#include <QObject>
#include <QString>
#include <qqmlregistration.h>

class QMimeData;
class QObject;
/*!
 * \qmltype MimeDataWrapper
 * \inqmlmodule org.kde.draganddrop
 * \brief Exposes a const QMimeData instance.
 *
 * In contrast to MimeData, doesn't create a copy of the QMimeData instance.
 */
class MimeDataWrapper : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("MimeDataWrapper cannot be created from QML.")

    /*!
     * \qmlproperty string MimeDataWrapper::text
     * A plain text (MIME type text/plain) representation of the data.
     */
    Q_PROPERTY(QString text READ text CONSTANT)

    /*!
     * \qmlproperty string MimeDataWrapper::html
     * A string if the data stored in the object is HTML (MIME type text/html); otherwise returns an empty string.
     */
    Q_PROPERTY(QString html READ html CONSTANT)

    /*!
     * \qmlproperty url MimeDataWrapper::url
     * Url contained in the mimedata
     */
    Q_PROPERTY(QUrl url READ url CONSTANT)

    /*!
     * \qmlproperty QJsonArray MimeDataWrapper::urls
     * A list of URLs contained within the MIME data object.
     * URLs correspond to the MIME type text/uri-list.
     */
    Q_PROPERTY(QJsonArray urls READ urls CONSTANT)

    /*!
     * \qmlproperty var MimeDataWrapper::color
     * A color if the data stored in the object represents a color (MIME type application/x-color); otherwise QVariant().
     */
    Q_PROPERTY(QVariant color READ color CONSTANT)

    /*!
     * \qmlproperty var MimeDataWrapper::source
     * The graphical item on the scene that started the drag event. It may be null.
     */
    Q_PROPERTY(QVariant source READ source CONSTANT)

    /*!
     * \qmlproperty list<string> MimeDataWrapper::formats
     * Mimetypes provided by the mime data instance
     *
     * \sa QMimeData::formats
     */
    Q_PROPERTY(QStringList formats READ formats CONSTANT)

    /*!
     * \qmlproperty bool MimeDataWrapper::hasUrls
     * \sa QMimeData::hasUrls
     */
    Q_PROPERTY(bool hasUrls READ hasUrls CONSTANT)

    /*!
     * \qmlproperty QMimeData* MimeDataWrapper::mimeData
     * Returns the wrapped object
     */
    Q_PROPERTY(QMimeData *mimeData READ mimeData CONSTANT)

public:
    MimeDataWrapper(const QMimeData *data, QObject *parent);

    QString text() const;
    QString html() const;
    QUrl url() const;
    QJsonArray urls() const;
    bool hasUrls() const;
    QVariant color() const;
    QStringList formats() const;
    QVariant source() const;
    QMimeData *mimeData() const;

    /*! \qmlmethod QByteArray MimeDataWrapper::getDataAsByteArray(string format) */
    Q_INVOKABLE QByteArray getDataAsByteArray(const QString &format);

private:
    const QMimeData *m_data;
};

#endif
