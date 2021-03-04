/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KDECLARATIVE_P_H
#define KDECLARATIVE_P_H

#include "kdeclarative.h"
#include "qmlobject.h"

#include <KLocalizedContext>
#include <QPointer>

namespace KDeclarative
{
class KDeclarativePrivate
{
public:
    KDeclarativePrivate();

    QPointer<QQmlEngine> declarativeEngine;
    QString translationDomain;
    static QStringList s_runtimePlatform;
    QPointer<KLocalizedContext> contextObj;
    QPointer<QmlObject> qmlObj;
};

}

#endif
