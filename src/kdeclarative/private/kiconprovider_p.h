/*
    SPDX-FileCopyrightText: 2011 Artur Duque de Souza <asouza@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <QQuickImageProvider>

namespace KDeclarative
{
class KIconProvider : public QQuickImageProvider
{
public:
    KIconProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};

}

#endif
