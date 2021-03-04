/*
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "qmlobjectsharedengine.h"

#include <QQmlContext>
#include <QQmlEngine>

#include <QDebug>
#include <kdeclarative.h>

namespace KDeclarative
{
class QmlObjectSharedEnginePrivate
{
public:
    QmlObjectSharedEnginePrivate()
    {
        // ensure the engine is present, then ref it
        engine();
        engineRef = s_engine;
    }

    ~QmlObjectSharedEnginePrivate()
    {
        // when the only remaining are out two refs, reset the pointers, causing deletion
        // when the refcount is 2, we are sure that the only refs are s_engine and our copy
        // of engineRef
        if (engineRef.use_count() == 2) {
            s_engine.reset();
        }
    }

    static QQmlEngine *engine()
    {
        if (!s_engine) {
            s_engine = std::make_shared<QQmlEngine>();
            KDeclarative::setupEngine(s_engine.get());
        }
        return s_engine.get();
    }

    // used to delete it
    std::shared_ptr<QQmlEngine> engineRef;

    static std::shared_ptr<QQmlEngine> s_engine;
};

std::shared_ptr<QQmlEngine> QmlObjectSharedEnginePrivate::s_engine = std::shared_ptr<QQmlEngine>();

QmlObjectSharedEngine::QmlObjectSharedEngine(QObject *parent)
    : QmlObject(QmlObjectSharedEnginePrivate::engine(), new QQmlContext(QmlObjectSharedEnginePrivate::engine()), this /*don't call setupEngine*/, parent)
    , d(new QmlObjectSharedEnginePrivate())
{
    rootContext()->setParent(this);
}

QmlObjectSharedEngine::~QmlObjectSharedEngine()
{
}

}

#include "moc_qmlobjectsharedengine.cpp"
