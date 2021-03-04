/*
    This file is part of the test suite of the Qt Toolkit.
    SPDX-FileCopyrightText: 2014 Digia Plc and/or its subsidiary(-ies) <http://www.qt-project.org/legal>

    SPDX-License-Identifier: LGPL-2.1-only WITH Qt-LGPL-exception-1.1 OR LGPL-3.0-only WITH Qt-LGPL-exception-1.1 OR LicenseRef-Qt-Commercial
*/

#ifndef QQMLTESTUTILS_H
#define QQMLTESTUTILS_H

#include <QObject>
#include <QStringList>
#include <QUrl>

QT_FORWARD_DECLARE_CLASS(QQmlComponent)
QT_FORWARD_DECLARE_CLASS(QQmlEngine)

/* Base class for tests with data that are located in a "data" subfolder. */

class QQmlDataTest : public QObject
{
    Q_OBJECT
public:
    QQmlDataTest();
    virtual ~QQmlDataTest();

    QString testFile(const QString &fileName) const;
    inline QString testFile(const char *fileName) const
    {
        return testFile(QLatin1String(fileName));
    }
    inline QUrl testFileUrl(const QString &fileName) const
    {
        return QUrl::fromLocalFile(testFile(fileName));
    }
    inline QUrl testFileUrl(const char *fileName) const
    {
        return testFileUrl(QLatin1String(fileName));
    }

    inline QString dataDirectory() const
    {
        return m_dataDirectory;
    }
    inline QUrl dataDirectoryUrl() const
    {
        return m_dataDirectoryUrl;
    }
    inline QString directory() const
    {
        return m_directory;
    }

    static inline QQmlDataTest *instance()
    {
        return m_instance;
    }

    static QByteArray msgComponentError(const QQmlComponent &, const QQmlEngine *engine = nullptr);

public Q_SLOTS:
    virtual void initTestCase();

private:
    static QQmlDataTest *m_instance;

    const QString m_dataDirectory;
    const QUrl m_dataDirectoryUrl;
    QString m_directory;
};

class QQmlTestMessageHandler
{
    Q_DISABLE_COPY(QQmlTestMessageHandler)
public:
    QQmlTestMessageHandler();
    ~QQmlTestMessageHandler();

    const QStringList &messages() const
    {
        return m_messages;
    }
    const QString messageString() const
    {
        return m_messages.join(QLatin1Char('\n'));
    }

    void clear()
    {
        m_messages.clear();
    }

private:
    static void messageHandler(QtMsgType, const QMessageLogContext &, const QString &message);

    static QQmlTestMessageHandler *m_instance;
    QStringList m_messages;
    QtMessageHandler m_oldHandler;
};

#endif // QQMLTESTUTILS_H
