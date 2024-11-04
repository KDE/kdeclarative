/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef MIMEDATAWRAPPER_H
#define MIMEDATAWRAPPER_H

#include <QJsonArray>
#include <QObject>
#include <QString>

class QMimeData;
class QObject;
/*!
 * \qmltype MimeDataWrapper
 * \inqmlmodule org.kde.draganddrop
 * Exposes a const QMimeData instance
 *
 * In contrast to DeclarativeMimeData, doesn't create a copy of the QMimeData instance
 */
class MimeDataWrapper : public QObject
{
    Q_OBJECT

    /*!
     * \qmlproperty QString MimeDataWrapper::text
     * A plain text (MIME type text/plain) representation of the data.
     */
    Q_PROPERTY(QString text READ text CONSTANT)

    /*!
     * \qmlproperty QString MimeDataWrapper::html
     * A string if the data stored in the object is HTML (MIME type text/html); otherwise returns an empty string.
     */
    Q_PROPERTY(QString html READ html CONSTANT)

    /*!
     * \qmlproperty QUrl MimeDataWrapper::url
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
     * \qmlproperty QVariant MimeDataWrapper::color
     * A color if the data stored in the object represents a color (MIME type application/x-color); otherwise QVariant().
     */
    Q_PROPERTY(QVariant color READ color CONSTANT)

    /*!
     * \qmlproperty QVariant MimeDataWrapper::source
     * The graphical item on the scene that started the drag event. It may be null.
     */
    Q_PROPERTY(QVariant source READ source CONSTANT)

    /*!
     * \qmlproperty QStringList MimeDataWrapper::formats
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

    /*! \qmlmethod QString MimeDataWrapper::text() */
    QString text() const;
    /*! \qmlmethod QString MimeDataWrapper::html() */
    QString html() const;
    /*! \qmlmethod QUrl MimeDataWrapper::url() */
    QUrl url() const;
    /*! \qmlmethod QJsonArray MimeDataWrapper::urls() */
    QJsonArray urls() const;
    /*! \qmlmethod bool MimeDataWrapper::hasUrls() */
    bool hasUrls() const;
    /*! \qmlmethod QVariant MimeDataWrapper::color() */
    QVariant color() const;
    /*! \qmlmethod QStringList MimeDataWrapper::formats() */
    QStringList formats() const;
    /*! \qmlmethod QVariant MimeDataWrapper::source() */
    QVariant source() const;
    /*! \qmlmethod QMimeData* MimeDataWrapper::mimeData() */
    QMimeData *mimeData() const;

    /*! \qmlmethod QByteArray MimeDataWrapper::getDataAsByteArray(const QString &format) */
    Q_INVOKABLE QByteArray getDataAsByteArray(const QString &format);

private:
    const QMimeData *m_data;
};

#endif
