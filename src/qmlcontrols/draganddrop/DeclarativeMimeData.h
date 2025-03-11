/*
    SPDX-FileCopyrightText: 2010 BetterInbox <contact@betterinbox.com>
    SPDX-FileContributor: Gregory Schlomoff <greg@betterinbox.com>

    SPDX-License-Identifier: MIT
*/

#ifndef DECLARATIVEMIMEDATA_H
#define DECLARATIVEMIMEDATA_H

#include <QColor>
#include <QJsonArray>
#include <QMimeData>
#include <QQuickItem>
#include <QUrl>

struct MimeDataForign {
    Q_GADGET
    QML_ANONYMOUS
    QML_FOREIGN(QMimeData)
};

/*!
 * \qmltype MimeData
 * \inqmlmodule org.kde.draganddrop
 *
 * \brief This is a wrapper class around QMimeData with a few extensions
 * to provide better support for in-qml drag & drops.
 */
class DeclarativeMimeData : public QMimeData
{
    Q_OBJECT
    QML_NAMED_ELEMENT(MimeData)
    QML_UNCREATABLE("MimeData cannot be created from QML.")

    /*!
     * \qmlproperty string MimeData::text
     * A plain text (MIME type text/plain) representation of the data.
     */
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

    /*!
     * \qmlproperty string MimeData::html
     * A string if the data stored in the object is HTML (MIME type text/html); otherwise returns an empty string.
     */
    Q_PROPERTY(QString html READ html WRITE setHtml NOTIFY htmlChanged)

    /// TODO: We should use QDeclarativeListProperty<QUrls> to return the whole list instead of only the first element.
    /*!
     * \qmlproperty url MimeData::url
     * The first URL from the urls property of QMimeData.
     */
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

    /*!
     * \qmlproperty QJsonArray MimeData::urls
     * A list of URLs contained within the MIME data object.
     * URLs correspond to the MIME type text/uri-list.
     */
    Q_PROPERTY(QJsonArray urls READ urls WRITE setUrls NOTIFY urlsChanged)

    /*!
     * \qmlproperty color MimeData::color
     * A color if the data stored in the object represents a color (MIME type application/x-color); otherwise QColor().
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /*!
     * \qmlproperty Item MimeData::source
     * The graphical item on the scene that started the drag event. It may be null.
     *
     * Setting source to any existing qml item will enable the receiver of the drag and drop operation to know in which item
     *  the operation originated.
     *
     *  In the case of inter-application drag and drop operations, the source will not be available, and will be 0.
     *  Be sure to test it in your QML code, before using it, or it will generate errors in the console.
     *
     */
    Q_PROPERTY(QQuickItem *source READ source WRITE setSource NOTIFY sourceChanged)

    /*!
     * \qmlproperty bool MimeData::hasUrls
     * \sa QMimeData::hasUrls
     */
    Q_PROPERTY(bool hasUrls READ hasUrls NOTIFY urlsChanged)
    // TODO: Image property

    /*!
     * \qmlproperty list<string> MimeData::formats
     * \sa QMimeData::formats
     */
    Q_PROPERTY(QStringList formats READ formats)
public:
    DeclarativeMimeData();
    /*!
     * \brief Creates a new MimeData by cloning the QMimeData
     * passed as parameter.
     *
     * This is useful for two reasons:
     * \list
     * \li In DragArea, we want to clone our "working copy" of the MimeData instance, as Qt will automatically delete it after the drag and drop operation.
     * \li In the drop events, the QMimeData is const, and we have troubles passing const to QML. So we clone it to remove the "constness"
     * \endlist
     *
     * This method will try to cast the QMimeData to MimeData, and will clone our extensions to QMimeData as well
     */
    DeclarativeMimeData(const QMimeData *copy);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QJsonArray urls() const;
    void setUrls(const QJsonArray &urls);

    QColor color() const;
    void setColor(const QColor &color);

    /*!
     * \qmlmethod MimeData::hasColor()
     */
    Q_INVOKABLE bool hasColor() const;

    /*!
     * \qmlmethod MimeData::setData(string mimeType, variant data)
     */
    Q_INVOKABLE void setData(const QString &mimeType, const QVariant &data);

    QQuickItem *source() const;
    void setSource(QQuickItem *source);

    /*!
     * \qmlmethod MimeData::getDataAsByteArray(string format)
     */
    Q_INVOKABLE QByteArray getDataAsByteArray(const QString &format);

    /*
    QString text() const;                //TODO: Reimplement this to issue the onChanged signals
    void setText(const QString &text);
    QString html() const;
    void setHtml(const QString &html);
    */

Q_SIGNALS:
    void textChanged(); // FIXME not being used
    void htmlChanged(); // FIXME not being used
    void urlChanged();
    void urlsChanged();
    void colorChanged();
    void sourceChanged();

private:
    QQuickItem *m_source;
};

#endif // DECLARATIVEMIMEDATA_H
