/*
    SPDX-FileCopyrightText: 2011 Artur Duque de Souza <asouza@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kiconprovider_p.h"

#include <KIconEffect>
#include <KIconLoader>
#include <QIcon>
#include <QPixmap>
#include <QSize>

namespace KDeclarative
{
KIconProvider::KIconProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap KIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    // We need to handle QIcon::state
    const QStringList source = id.split(QLatin1Char('/'));

    QPixmap pixmap;
    if (requestedSize.isValid()) {
        pixmap = QIcon::fromTheme(source.at(0)).pixmap(requestedSize);
    } else if (size->isValid()) {
        pixmap = QIcon::fromTheme(source.at(0)).pixmap(*size);
    } else {
        pixmap = QIcon::fromTheme(source.at(0)).pixmap(KIconLoader::global()->currentSize(KIconLoader::Desktop));
    }

    if (source.size() == 2) {
        KIconEffect *effect = KIconLoader::global()->iconEffect();
        const QString state(source.at(1));
        int finalState = KIconLoader::DefaultState;

        if (state == QLatin1String("active")) {
            finalState = KIconLoader::ActiveState;
        } else if (state == QLatin1String("disabled")) {
            finalState = KIconLoader::DisabledState;
        } else if (state == QLatin1String("last")) {
            finalState = KIconLoader::LastState;
        }

        // apply the effect for state
        pixmap = effect->apply(pixmap, KIconLoader::Desktop, finalState);
    }

    if (!pixmap.isNull() && size) {
        *size = pixmap.size();
    }

    return pixmap;
}

}
