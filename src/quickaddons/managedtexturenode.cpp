/*
    SPDX-FileCopyrightText: 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managedtexturenode.h"

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)

ManagedTextureNode::ManagedTextureNode()
{
}

void ManagedTextureNode::setTexture(QSharedPointer<QSGTexture> texture)
{
    m_texture = texture;
    QSGSimpleTextureNode::setTexture(texture.data());
}

#endif
