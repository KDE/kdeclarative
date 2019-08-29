/***************************************************************************
 Copyright 2011 Artur Duque de Souza <asouza@kde.org>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "kiconprovider_p.h"

#include <QPixmap>
#include <QSize>
#include <QIcon>
#include <kiconloader.h>
#include <kiconeffect.h>

namespace KDeclarative {

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
        pixmap = QIcon::fromTheme(source.at(0)).pixmap(IconSize(KIconLoader::Desktop));
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

