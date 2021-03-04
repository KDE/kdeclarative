/*
    SPDX-FileCopyrightText: 2014 Antonis Tsiapaliokas <antonis.tsiapaliokas@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KRUNPROXY_H
#define KRUNPROXY_H

#include <QObject>

class KRunProxy : public QObject
{
    Q_OBJECT

public:
    explicit KRunProxy(QObject *parent = nullptr);

    /**
     * opens the url from the existing file
     */
    Q_INVOKABLE bool openUrl(const QString &fileUrl);

    /**
     * Runs the service (application)
     * @param serviceName the name of the desktop file without extension
     * @return true on success
     */
    Q_INVOKABLE bool openService(const QString &serviceName);
};

#endif
