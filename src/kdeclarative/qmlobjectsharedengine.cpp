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

#include "qmlobjectsharedengine.h"
#include "private/qmlobject_p.h"

#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QPointer>

#include <qdebug.h>
#include <kdeclarative.h>

namespace KDeclarative {

class QmlObjectSharedEnginePrivate
{
public:
    QmlObjectSharedEnginePrivate()
    {
        //ensure the engine is present, then ref it
        engine();
        engineRef = s_engine;
    }

    ~QmlObjectSharedEnginePrivate()
    {
        //when the only remaining are out two refs, reset the pointers, causing deletion
        //when the refcount is 2, we are sure that the only refs are s_engine and our copy
        //of engineRef
        if (engineRef.use_count() == 2) {
            s_engine.reset();
        }
    }

    static QQmlEngine *engine()
    {
        if (!s_engine) {
            s_engine = std::make_shared<QQmlEngine>();
            s_engine->setIncubationController(new QmlObjectIncubationController(0));            
        }
        return s_engine.get();
    }

    //used to delete it
    std::shared_ptr<QQmlEngine> engineRef;

    static std::shared_ptr<QQmlEngine> s_engine;
};

std::shared_ptr<QQmlEngine> QmlObjectSharedEnginePrivate::s_engine = std::shared_ptr<QQmlEngine>();

QmlObjectSharedEngine::QmlObjectSharedEngine(QObject *parent)
    : QmlObject(QmlObjectSharedEnginePrivate::engine(), new QQmlContext(QmlObjectSharedEnginePrivate::engine()), parent),
      d(new QmlObjectSharedEnginePrivate())
{
}

QmlObjectSharedEngine::~QmlObjectSharedEngine()
{
    rootContext()->deleteLater();
}


}

#include "moc_qmlobjectsharedengine.cpp"
