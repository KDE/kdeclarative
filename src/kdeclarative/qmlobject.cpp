/*
 *   Copyright 2013 Marco Martin <mart@kde.org>
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

#include "qmlobject.h"
#include "private/qmlobject_p.h"
#include "private/kdeclarative_p.h"

#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlIncubator>
#include <QTimer>
#include <QPointer>

#include <qdebug.h>
#include <kdeclarative.h>
#include <KPackage/PackageLoader>

//#include "packageaccessmanagerfactory.h"
//#include "private/declarative/dataenginebindings_p.h"

namespace KDeclarative {

class QmlObjectIncubator : public QQmlIncubator
{
public:
    QVariantHash m_initialProperties;
protected:
    void setInitialState(QObject *object) Q_DECL_OVERRIDE
    {
        QHashIterator<QString, QVariant> i(m_initialProperties);
        while (i.hasNext()) {
            i.next();
            object->setProperty(i.key().toLatin1().data(), i.value());
        }
    }
};

class QmlObjectPrivate
{
public:
    QmlObjectPrivate(QmlObject *parent)
        : q(parent),
          engine(0),
          component(0),
          delay(false)
    {
        executionEndTimer = new QTimer(q);
        executionEndTimer->setInterval(0);
        executionEndTimer->setSingleShot(true);
        QObject::connect(executionEndTimer, SIGNAL(timeout()), q, SLOT(scheduleExecutionEnd()));
    }

    ~QmlObjectPrivate()
    {
        delete incubator.object();
    }

    void errorPrint(QQmlComponent *component);
    void execute(const QUrl &source);
    void scheduleExecutionEnd();
    void minimumWidthChanged();
    void minimumHeightChanged();
    void maximumWidthChanged();
    void maximumHeightChanged();
    void preferredWidthChanged();
    void preferredHeightChanged();
    void checkInitializationCompleted();

    QmlObject *q;

    QUrl source;
    QQmlEngine *engine;
    QmlObjectIncubator incubator;
    QQmlComponent *component;
    QTimer *executionEndTimer;
    KDeclarative kdeclarative;
    KPackage::Package package;
    QQmlContext *rootContext;
    bool delay : 1;
};

void QmlObjectPrivate::errorPrint(QQmlComponent *component)
{
    QString errorStr = QStringLiteral("Error loading QML file.\n");
    if (component->isError()) {
        QList<QQmlError> errors = component->errors();
        foreach (const QQmlError &error, errors) {
            errorStr += (error.line() > 0 ? QString(QString::number(error.line()) + QLatin1String(": ")) : QLatin1String(""))
                        + error.description() + QLatin1Char('\n');
        }
    }
    qWarning() << component->url().toString() << '\n' << errorStr;
}

void QmlObjectPrivate::execute(const QUrl &source)
{
    if (source.isEmpty()) {
        qWarning() << "File name empty!";
        return;
    }

    delete component;
    component = new QQmlComponent(engine, q);
    QObject::connect(component, &QQmlComponent::statusChanged,
                     q, &QmlObject::statusChanged, Qt::QueuedConnection);
    delete incubator.object();

    component->loadUrl(source);

    if (delay) {
        executionEndTimer->start(0);
    } else {
        scheduleExecutionEnd();
    }
}

void QmlObjectPrivate::scheduleExecutionEnd()
{
    if (component->isReady() || component->isError()) {
        q->completeInitialization();
    } else {
        QObject::connect(component, SIGNAL(statusChanged(QQmlComponent::Status)), q, SLOT(completeInitialization()));
    }
}

QmlObject::QmlObject(QObject *parent)
    : QObject(parent),
      d(new QmlObjectPrivate(this))
{
    d->engine = new QQmlEngine(this);
    d->rootContext = d->engine->rootContext();
    d->kdeclarative.setDeclarativeEngine(d->engine);
    d->kdeclarative.d->qmlObj = this;
    //binds things like kconfig and icons
    d->kdeclarative.setupBindings();
    d->engine->setIncubationController(new QmlObjectIncubationController(this));
}

QmlObject::QmlObject(QQmlEngine *engine, QObject *parent)
    : QObject(parent),
      d(new QmlObjectPrivate(this))
{
    if (engine) {
        d->engine = engine;
    } else {
        d->engine = new QQmlEngine(this);
        d->engine->setIncubationController(new QmlObjectIncubationController(this));
    }
    d->rootContext = d->engine->rootContext();
    d->kdeclarative.setDeclarativeEngine(d->engine);
    d->kdeclarative.d->qmlObj = this;
    //binds things like kconfig and icons
    d->kdeclarative.setupBindings();
}

QmlObject::QmlObject(QQmlEngine *engine, QQmlContext *rootContext, QObject *parent)
    : QObject(parent),
      d(new QmlObjectPrivate(this))
{
    if (engine) {
        d->engine = engine;
    } else {
        d->engine = new QQmlEngine(this);
        d->engine->setIncubationController(new QmlObjectIncubationController(0));
    }

    if (rootContext) {
        d->rootContext = rootContext;
    } else {
        d->rootContext = d->engine->rootContext();
    }
    d->kdeclarative.setDeclarativeEngine(d->engine);
    d->kdeclarative.d->qmlObj = this;
    //binds things like kconfig and icons
    d->kdeclarative.setupBindings();
}

QmlObject::~QmlObject()
{
//    QDeclarativeNetworkAccessManagerFactory *factory = d->engine->networkAccessManagerFactory();
//    d->engine->setNetworkAccessManagerFactory(0);
//    delete factory;
    delete d;
}

void QmlObject::setTranslationDomain(const QString &translationDomain)
{
    d->kdeclarative.setTranslationDomain(translationDomain);
}

QString QmlObject::translationDomain() const
{
    return d->kdeclarative.translationDomain();
}

void QmlObject::setSource(const QUrl &source)
{
    d->source = source;
    d->execute(source);
}

QUrl QmlObject::source() const
{
    return d->source;
}

void QmlObject::loadPackage(const QString &packageName)
{
    d->package = KPackage::PackageLoader::self()->loadPackage("KPackage/GenericQML");
    d->package.setPath(packageName);
    setSource(QUrl::fromLocalFile(d->package.filePath("mainscript")));
}

void QmlObject::setPackage(const KPackage::Package &package)
{
    d->package = package;
    setSource(QUrl::fromLocalFile(package.filePath("mainscript")));
}

KPackage::Package QmlObject::package() const
{
    return d->package;
}

void QmlObject::setInitializationDelayed(const bool delay)
{
    d->delay = delay;
}

bool QmlObject::isInitializationDelayed() const
{
    return d->delay;
}

QQmlEngine *QmlObject::engine()
{
    return d->engine;
}

QObject *QmlObject::rootObject() const
{
    if (d->incubator.status() == QQmlIncubator::Loading) {
        qWarning() << "Trying to use rootObject before initialization is completed, whilst using setInitializationDelayed. Forcing completion";
        d->incubator.forceCompletion();
    }
    return d->incubator.object();
}

QQmlComponent *QmlObject::mainComponent() const
{
    return d->component;
}

QQmlContext *QmlObject::rootContext() const
{
    return d->rootContext;
}

QQmlComponent::Status QmlObject::status() const
{
    if (!d->engine) {
        return QQmlComponent::Error;
    }

    if (!d->component) {
        return QQmlComponent::Null;
    }

    return QQmlComponent::Status(d->component->status());
}

void QmlObjectPrivate::checkInitializationCompleted()
{
    if (!incubator.isReady() && incubator.status() != QQmlIncubator::Error) {
        QTimer::singleShot(0, q, SLOT(checkInitializationCompleted()));
        return;
    }

    if (!incubator.object()) {
        errorPrint(component);
    }

    emit q->finished();
}

void QmlObject::completeInitialization(const QVariantHash &initialProperties)
{
    d->executionEndTimer->stop();
    if (d->incubator.object()) {
        return;
    }

    if (!d->component) {
        qWarning() << "No component for" << source();
        return;
    }


    if (d->component->status() != QQmlComponent::Ready || d->component->isError()) {
        d->errorPrint(d->component);
        return;
    }

    d->incubator.m_initialProperties = initialProperties;
    d->component->create(d->incubator, d->rootContext);

    if (d->delay) {
        d->checkInitializationCompleted();
    } else {
        d->incubator.forceCompletion();

        if (!d->incubator.object()) {
            d->errorPrint(d->component);
        }
        emit finished();
    }
}

QObject *QmlObject::createObjectFromSource(const QUrl &source, QQmlContext *context, const QVariantHash &initialProperties)
{
    QQmlComponent *component = new QQmlComponent(d->engine, this);
    component->loadUrl(source);

    return createObjectFromComponent(component, context, initialProperties);
}

QObject *QmlObject::createObjectFromComponent(QQmlComponent *component, QQmlContext *context, const QVariantHash &initialProperties)
{
    QmlObjectIncubator incubator;
    incubator.m_initialProperties = initialProperties;
    component->create(incubator, context ? context : d->rootContext);
    incubator.forceCompletion();
  
    QObject *object = incubator.object();

    if (!component->isError() && object) {
        //memory management
        component->setParent(object);
        //reparent to root object if wasn't specified otherwise by initialProperties
        if (!initialProperties.contains("parent")) {
            if (qobject_cast<QQuickItem *>(rootObject())) {
                object->setProperty("parent", QVariant::fromValue(rootObject()));
            } else {
                object->setParent(rootObject());
            }
        }

        return object;

    } else {
        d->errorPrint(component);
        delete object;
        return 0;
    }
}

}

#include "moc_qmlobject.cpp"
