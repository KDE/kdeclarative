/*
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef QMLOBJECTSHAREDENGINE_H
#define QMLOBJECTSHAREDENGINE_H

#include <kdeclarative/kdeclarative_export.h>
#include <kdeclarative/qmlobject.h>
#include <memory>

namespace KDeclarative
{
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
    ~QmlObjectSharedEngine() override;

private:
    friend class QmlObjectSharedEnginePrivate;
    const std::unique_ptr<QmlObjectSharedEnginePrivate> d;
};

}

#endif // multiple inclusion guard
