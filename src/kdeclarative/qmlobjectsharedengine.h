/*
 *   Copyright 2015 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef QMLOBJECTSHAREDENGINE_H
#define QMLOBJECTSHAREDENGINE_H

#include <memory>
#include <kdeclarative/qmlobject.h>
#include <kdeclarative/kdeclarative_export.h>

namespace KDeclarative {

class QmlObjectSharedEnginePrivate;

/**
 * @class KDeclarative::QmlObjectSharedEngine qmlobjectsharedengine.h KDeclarative/QmlObjectSharedEngine
 *
 * @author Marco Martin <mart@kde.org>
 *
 * @short An object that instantiates an entire QML context, with its own declarative engine
 *
 * KDeclarative::QmlObjectSharedEngine is a version of QmlObject that ensures only one
 * QQmlEngine instance exists for the whole application. Objects created by different
 * instances of QmlObjectSharedEngine will be insulated by having different creation 
 * contexts, accessible by QmlObject::rootContext()
 */
class KDECLARATIVE_EXPORT QmlObjectSharedEngine : public QmlObject
{
    Q_OBJECT

public:
    /**
     * Constructs a new QmlObjectSharedEngine
     *
     * @param parent the parent of this object
     */
    explicit QmlObjectSharedEngine(QObject *parent = nullptr);
    ~QmlObjectSharedEngine();

private:
    friend class QmlObjectSharedEnginePrivate;
    const std::unique_ptr<QmlObjectSharedEnginePrivate> d;
};

}

#endif // multiple inclusion guard
