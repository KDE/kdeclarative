/*
    SPDX-FileCopyrightText: 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "imagetexturescache.h"
#include <QOpenGLContext>
#include <QSGTexture>

typedef QHash<qint64, QHash<QWindow *, QWeakPointer<QSGTexture>>> TexturesCache;

class ImageTexturesCachePrivate
{
public:
    TexturesCache cache;
};

ImageTexturesCache::ImageTexturesCache()
    : d(new ImageTexturesCachePrivate)
{
}

ImageTexturesCache::~ImageTexturesCache()
{
}

QSharedPointer<QSGTexture> ImageTexturesCache::loadTexture(QQuickWindow *window, const QImage &image, QQuickWindow::CreateTextureOptions options)
{
    QQuickWindow *cacheWindowKey = window;
    // if we are using shared contexts, we don't need a per window cache
    if (QOpenGLContext::areSharing(QOpenGLContext::currentContext(), QOpenGLContext::globalShareContext())) {
        cacheWindowKey = nullptr;
    }

    qint64 id = image.cacheKey();
    QSharedPointer<QSGTexture> texture = d->cache.value(id).value(cacheWindowKey).toStrongRef();

    if (!texture) {
        auto cleanAndDelete = [this, cacheWindowKey, id](QSGTexture *texture) {
            QHash<QWindow *, QWeakPointer<QSGTexture>> &textures = (d->cache)[id];
            textures.remove(cacheWindowKey);
            if (textures.isEmpty()) {
                d->cache.remove(id);
            }
            delete texture;
        };
        texture = QSharedPointer<QSGTexture>(window->createTextureFromImage(image, options), cleanAndDelete);
        (d->cache)[id][cacheWindowKey] = texture.toWeakRef();
    }

    // if we have a cache in an atlas but our request cannot use an atlassed texture
    // create a new texture and use that
    // don't use removedFromAtlas() as that requires keeping a reference to the non atlased version
    if (!(options & QQuickWindow::TextureCanUseAtlas) && texture->isAtlasTexture()) {
        texture = QSharedPointer<QSGTexture>(window->createTextureFromImage(image, options));
    }

    return texture;
}

QSharedPointer<QSGTexture> ImageTexturesCache::loadTexture(QQuickWindow *window, const QImage &image)
{
    return loadTexture(window, image, QQuickWindow::CreateTextureOptions());
}
