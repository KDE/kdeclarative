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

/*!
 * \qmltype DeclarativeMimeData
 * \inqmlmodule org.kde.draganddrop
 *
 * \brief This is a wrapper class around QMimeData with a few extensions
 * to provide better support for in-qml drag & drops.
 *
 */
class DeclarativeMimeData : public QMimeData
{
    Q_OBJECT

    /*!
     * \qmlproperty QString DeclarativeMimeData::text
     * A plain text (MIME type text/plain) representation of the data.
     */
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

    /*!
     * \qmlproperty QString DeclarativeMimeData::html
     * A string if the data stored in the object is HTML (MIME type text/html); otherwise returns an empty string.
     */
    Q_PROPERTY(QString html READ html WRITE setHtml NOTIFY htmlChanged)

    /// TODO: We should use QDeclarativeListProperty<QUrls> to return the whole list instead of only the first element.
    /*!
     * \qmlproperty QUrl DeclarativeMimeData::url
     * The first URL from the urls property of QMimeData.
     */
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

    /*!
     * \qmlproperty QJsonArray DeclarativeMimeData::urls
     * A list of URLs contained within the MIME data object.
     * URLs correspond to the MIME type text/uri-list.
     */
    Q_PROPERTY(QJsonArray urls READ urls WRITE setUrls NOTIFY urlsChanged)

    /*!
     * \qmlproperty QColor DeclarativeMimeData::color
     * A color if the data stored in the object represents a color (MIME type application/x-color); otherwise QColor().
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /*!
     * \qmlproperty QQuickItem* DeclarativeMimeData::source
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
     * \qmlproperty bool DeclarativeMimeData::hasUrls
     * \sa QMimeData::hasUrls
     */
    Q_PROPERTY(bool hasUrls READ hasUrls NOTIFY urlsChanged)
    // TODO: Image property

    /*!
     * \qmlproperty QStringList DeclarativeMimeData::formats
     * \sa QMimeData::formats
     */
    Q_PROPERTY(QStringList formats READ formats)
public:
    DeclarativeMimeData();
    /*!
     * \brief Creates a new DeclarativeMimeData by cloning the QMimeData
     * passed as parameter.
     *
     * This is useful for two reasons :
     *        - In DragArea, we want to clone our "working copy" of the DeclarativeMimeData instance, as Qt will automatically
     *        delete it after the drag and drop operation.
     *        - In the drop events, the QMimeData is const, and we have troubles passing const to QML. So we clone it to
     *        remove the "constness"
     *
     * This method will try to cast the QMimeData to DeclarativeMimeData, and will clone our extensions to QMimeData as well
     */
    DeclarativeMimeData(const QMimeData *copy);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QJsonArray urls() const;
    void setUrls(const QJsonArray &urls);

    QColor color() const;
    void setColor(const QColor &color);
    Q_INVOKABLE bool hasColor() const;

    Q_INVOKABLE void setData(const QString &mimeType, const QVariant &data);

    QQuickItem *source() const;
    void setSource(QQuickItem *source);

    Q_INVOKABLE QByteArray getDataAsByteArray(const QString &format);

    /*
    QString text() const;                //TODO: Reimplement this to issue the onChanged signals
    void setText(const QString &text);
    QString html() const;
    void setHtml(const QString &html);
    */

Q_SIGNALS:
    /*! \qmlsignal DeclarativeMimeData::textChanged */
    void textChanged(); // FIXME not being used
    /*! \qmlsignal DeclarativeMimeData::htmlChanged */
    void htmlChanged(); // FIXME not being used
    /*! \qmlsignal DeclarativeMimeData::urlChanged */
    void urlChanged();
    /*! \qmlsignal DeclarativeMimeData::urlsChanged */
    void urlsChanged();
    /*! \qmlsignal DeclarativeMimeData::colorChanged */
    void colorChanged();
    /*! \qmlsignal DeclarativeMimeData::sourceChanged */
    void sourceChanged();

private:
    QQuickItem *m_source;
};

#endif // DECLARATIVEMIMEDATA_H
