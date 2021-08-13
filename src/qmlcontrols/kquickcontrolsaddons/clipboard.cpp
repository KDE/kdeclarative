/*
    SPDX-FileCopyrightText: 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "clipboard.h"
#include <QDebug>
#include <QGuiApplication>
#include <QMimeData>
#include <QUrl>

Clipboard::Clipboard(QObject *parent)
    : QObject(parent)
    , m_clipboard(QGuiApplication::clipboard())
    , m_mode(QClipboard::Clipboard)
{
    connect(m_clipboard, SIGNAL(changed(QClipboard::Mode)), SLOT(clipboardChanged(QClipboard::Mode)));
}

void Clipboard::setMode(QClipboard::Mode mode)
{
    m_mode = mode;
    Q_EMIT modeChanged(m_mode);
}

void Clipboard::clipboardChanged(QClipboard::Mode m)
{
    if (m == m_mode) {
        Q_EMIT contentChanged();
    }
}

void Clipboard::clear()
{
    m_clipboard->clear(m_mode);
}

QClipboard::Mode Clipboard::mode() const
{
    return m_mode;
}

QVariant Clipboard::contentFormat(const QString &format) const
{
    const QMimeData *data = m_clipboard->mimeData(m_mode);
    QVariant ret;
    if (format == QLatin1String("text/uri-list")) {
        QVariantList retList;
        const auto urls = data->urls();
        for (const QUrl &url : urls) {
            retList += url;
        }
        ret = retList;
    } else if (format.startsWith(QLatin1String("text/"))) {
        ret = data->text();
    } else if (format.startsWith(QLatin1String("image/"))) {
        ret = data->imageData();
    } else {
        ret = data->data(format.isEmpty() ? data->formats().first() : format);
    }

    return ret;
}

QVariant Clipboard::content() const
{
    return contentFormat(m_clipboard->mimeData(m_mode)->formats().first());
}

void Clipboard::setContent(const QVariant &content)
{
    QMimeData *mimeData = new QMimeData;
    switch (content.type()) {
    case QVariant::String:
        mimeData->setText(content.toString());
        break;
    case QVariant::Color:
        mimeData->setColorData(content.toString());
        break;
    case QVariant::Pixmap:
    case QVariant::Image:
        mimeData->setImageData(content);
        break;
    default:
        if (content.type() == QVariant::List) {
            const QVariantList list = content.toList();
            QList<QUrl> urls;
            bool wasUrlList = true;
            for (const QVariant &url : list) {
                if (url.type() != QVariant::Url) {
                    wasUrlList = false;
                    break;
                }
                urls += url.toUrl();
            }
            if (wasUrlList) {
                mimeData->setUrls(urls);
                break;
            }
        }

        if (content.canConvert(QVariant::String)) {
            mimeData->setText(content.toString());
        } else {
            mimeData->setData(QStringLiteral("application/octet-stream"), content.toByteArray());
            qWarning() << "Couldn't figure out the content type, storing as application/octet-stream";
        }
        break;
    }
    m_clipboard->setMimeData(mimeData, m_mode);
}

QStringList Clipboard::formats() const
{
    return m_clipboard->mimeData(m_mode)->formats();
}
