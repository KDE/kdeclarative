/*
    This file is part of the test suite of the Qt Toolkit.
    SPDX-FileCopyrightText: 2014 Digia Plc and/or its subsidiary(-ies) <http://www.qt-project.org/legal>

    SPDX-License-Identifier: LGPL-2.1-only WITH Qt-LGPL-exception-1.1 OR LGPL-3.0-only WITH Qt-LGPL-exception-1.1 OR LicenseRef-Qt-Commercial
*/

#include "util.h"

#include <QMutexLocker>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlError>
#include <QTest>
#include <QTextStream>

QQmlDataTest *QQmlDataTest::m_instance = nullptr;

QQmlDataTest::QQmlDataTest()
    : m_dataDirectory(QFINDTESTDATA("input"))
    , m_dataDirectoryUrl(QUrl::fromLocalFile(m_dataDirectory + QLatin1Char('/')))
{
    m_instance = this;
}

QQmlDataTest::~QQmlDataTest()
{
    m_instance = nullptr;
}

void QQmlDataTest::initTestCase()
{
    QVERIFY2(!m_dataDirectory.isEmpty(), "'input' directory not found");
    m_directory = QFileInfo(m_dataDirectory).absolutePath();
    QVERIFY2(QDir::setCurrent(m_directory), qPrintable(QLatin1String("Could not chdir to ") + m_directory));
}

QString QQmlDataTest::testFile(const QString &fileName) const
{
    if (m_directory.isEmpty()) {
        qFatal("QQmlDataTest::initTestCase() not called.");
    }
    QString result = m_dataDirectory;
    result += QLatin1Char('/');
    result += fileName;
    return result;
}

QByteArray QQmlDataTest::msgComponentError(const QQmlComponent &c, const QQmlEngine *engine /* = 0 */)
{
    QString result;
    const QList<QQmlError> errors = c.errors();
    QTextStream str(&result);
    str << "Component '" << c.url().toString() << "' has " << errors.size() << " errors: '";
    for (int i = 0; i < errors.size(); ++i) {
        if (i) {
            str << ", '";
        }
        str << errors.at(i).toString() << '\'';
    }
    if (!engine) {
        if (QQmlContext *context = c.creationContext()) {
            engine = context->engine();
        }
    }
    if (engine) {
        str << " Import paths: (" << engine->importPathList().join(QLatin1String(", ")) << ") Plugin paths: ("
            << engine->pluginPathList().join(QLatin1String(", ")) << ')';
    }
    return result.toLocal8Bit();
}

Q_GLOBAL_STATIC(QMutex, qQmlTestMessageHandlerMutex)

QQmlTestMessageHandler *QQmlTestMessageHandler::m_instance = nullptr;

void QQmlTestMessageHandler::messageHandler(QtMsgType, const QMessageLogContext &, const QString &message)
{
    QMutexLocker locker(qQmlTestMessageHandlerMutex());
    if (QQmlTestMessageHandler::m_instance) {
        QQmlTestMessageHandler::m_instance->m_messages.push_back(message);
    }
}

QQmlTestMessageHandler::QQmlTestMessageHandler()
{
    QMutexLocker locker(qQmlTestMessageHandlerMutex());
    Q_ASSERT(!QQmlTestMessageHandler::m_instance);
    QQmlTestMessageHandler::m_instance = this;
    m_oldHandler = qInstallMessageHandler(messageHandler);
}

QQmlTestMessageHandler::~QQmlTestMessageHandler()
{
    QMutexLocker locker(qQmlTestMessageHandlerMutex());
    Q_ASSERT(QQmlTestMessageHandler::m_instance);
    qInstallMessageHandler(m_oldHandler);
    QQmlTestMessageHandler::m_instance = nullptr;
}
