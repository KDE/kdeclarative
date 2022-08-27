/*
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "qmlobjectsharedengine.h"

#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlNetworkAccessManagerFactory>

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
        // when the only remaining are our three refs, reset the pointers, causing deletion
        // when the refcount is 3, we are sure that the only refs are s_engine, our copy
        // of engineRef and m_engine in QmlObject.
        if (engineRef.use_count() <= 3) {
            s_engine.reset();
        }
    }

    static std::shared_ptr<QQmlEngine> engine()
    {
        if (!s_engine) {
            s_engine = std::make_shared<QQmlEngine>();
#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 98)
            KDeclarative::setupEngine(s_engine.get());
#endif
        }
        return s_engine;
    }

    // used to delete it
    std::shared_ptr<QQmlEngine> engineRef;

    static std::shared_ptr<QQmlEngine> s_engine;
};

std::shared_ptr<QQmlEngine> QmlObjectSharedEnginePrivate::s_engine = std::shared_ptr<QQmlEngine>();

QmlObjectSharedEngine::QmlObjectSharedEngine(QObject *parent)
    : QmlObject(QmlObjectSharedEnginePrivate::engine(), new QQmlContext(QmlObjectSharedEnginePrivate::engine().get()), parent)
    , d(new QmlObjectSharedEnginePrivate())
{
    rootContext()->setParent(this);
}

QmlObjectSharedEngine::~QmlObjectSharedEngine()
{
}

}

#include "moc_qmlobjectsharedengine.cpp"
