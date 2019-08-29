/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "util.h"

#include <QQmlComponent>
#include <QQmlError>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTextStream>
#include <QMutexLocker>
#include <QTest>

QQmlDataTest *QQmlDataTest::m_instance = nullptr;

QQmlDataTest::QQmlDataTest() :
    m_dataDirectory(QFINDTESTDATA("data")),

    m_dataDirectoryUrl(QUrl::fromLocalFile(m_dataDirectory + QLatin1Char('/')))
{
    m_instance = this;
}

QQmlDataTest::~QQmlDataTest()
{
    m_instance = nullptr;
}

void QQmlDataTest::initTestCase()
{
    QVERIFY2(!m_dataDirectory.isEmpty(), "'data' directory not found");
    m_directory = QFileInfo(m_dataDirectory).absolutePath();
    QVERIFY2(QDir::setCurrent(m_directory), qPrintable(QLatin1String("Could not chdir to ") + m_directory));
}

QString QQmlDataTest::testFile(const QString &fileName) const
{
    if (m_directory.isEmpty())
        qFatal("QQmlDataTest::initTestCase() not called.");
    QString result = m_dataDirectory;
    result += QLatin1Char('/');
    result += fileName;
    return result;
}

QByteArray QQmlDataTest::msgComponentError(const QQmlComponent &c,
                                                   const QQmlEngine *engine /* = 0 */)
{
    QString result;
    const QList<QQmlError> errors = c.errors();
    QTextStream str(&result);
    str << "Component '" << c.url().toString() << "' has " << errors.size()
        << " errors: '";
    for (int i = 0; i < errors.size(); ++i) {
        if (i)
            str << ", '";
        str << errors.at(i).toString() << '\'';

    }
    if (!engine)
        if (QQmlContext *context = c.creationContext())
            engine = context->engine();
    if (engine) {
        str << " Import paths: (" << engine->importPathList().join(QLatin1String(", "))
            << ") Plugin paths: (" << engine->pluginPathList().join(QLatin1String(", "))
            << ')';
    }
    return result.toLocal8Bit();
}

Q_GLOBAL_STATIC(QMutex, qQmlTestMessageHandlerMutex)

QQmlTestMessageHandler *QQmlTestMessageHandler::m_instance = nullptr;

void QQmlTestMessageHandler::messageHandler(QtMsgType, const QMessageLogContext &, const QString &message)
{
    QMutexLocker locker(qQmlTestMessageHandlerMutex());
    if (QQmlTestMessageHandler::m_instance)
        QQmlTestMessageHandler::m_instance->m_messages.push_back(message);
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
