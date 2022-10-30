/*
    SPDX-FileCopyrightText: 2013 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "qmlobject.h"
#include "private/kdeclarative_p.h"

#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlIncubator>
#include <QQmlNetworkAccessManagerFactory>
#include <QQuickItem>
#include <QTimer>

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 98)
#include <KPackage/PackageLoader>
#endif
#include <QDebug>
#include <kdeclarative.h>

//#include "packageaccessmanagerfactory.h"
//#include "private/declarative/dataenginebindings_p.h"

namespace KDeclarative
{
class QmlObjectIncubator : public QQmlIncubator
{
public:
    QVariantHash m_initialProperties;

protected:
    void setInitialState(QObject *object) override
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
        : q(parent)
        , engine(nullptr)
        , component(nullptr)
        , delay(false)
    {
        executionEndTimer = new QTimer(q);
        executionEndTimer->setInterval(0);
        executionEndTimer->setSingleShot(true);
        QObject::connect(executionEndTimer, &QTimer::timeout, q, [this]() {
            scheduleExecutionEnd();
        });
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
    std::shared_ptr<QQmlEngine> engine;

    QmlObjectIncubator incubator;
    QQmlComponent *component;
    QTimer *executionEndTimer;
    KLocalizedContext *context{nullptr};
#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 98)
    KPackage::Package package;
#endif
    QQmlContext *rootContext;
    bool delay : 1;
};

void QmlObjectPrivate::errorPrint(QQmlComponent *component)
{
    QString errorStr = QStringLiteral("Error loading QML file.\n");
    if (component->isError()) {
        const QList<QQmlError> errors = component->errors();
        for (const QQmlError &error : errors) {
            errorStr +=
                (error.line() > 0 ? QString(QString::number(error.line()) + QLatin1String(": ")) : QLatin1String("")) + error.description() + QLatin1Char('\n');
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
    component = new QQmlComponent(engine.get(), q);
    QObject::connect(component, &QQmlComponent::statusChanged, q, &QmlObject::statusChanged, Qt::QueuedConnection);
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
        QObject::connect(component, &QQmlComponent::statusChanged, q, [this]() {
            q->completeInitialization();
        });
    }
}

QmlObject::QmlObject(QObject *parent)
    : QmlObject(nullptr, nullptr, parent)
{
}

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 95)
QmlObject::QmlObject(QQmlEngine *engine, QObject *parent)
    : QmlObject(std::shared_ptr<QQmlEngine>(engine), nullptr, parent)
{
}

QmlObject::QmlObject(QQmlEngine *engine, QQmlContext *rootContext, QObject *parent)
    : QmlObject(std::shared_ptr<QQmlEngine>(engine), rootContext, parent)
{
}

QmlObject::QmlObject(QQmlEngine *engine, QQmlContext *rootContext, QmlObject *obj, QObject *parent)
    : QmlObject(std::shared_ptr<QQmlEngine>(engine), rootContext, parent)
{
    Q_UNUSED(obj);
}
#endif

QmlObject::QmlObject(std::shared_ptr<QQmlEngine> engine, QQmlContext *rootContext, QObject *parent)
    : QObject(parent)
    , d(new QmlObjectPrivate(this))
{
    if (engine) {
        d->engine = engine;
    } else {
        d->engine = std::make_shared<QQmlEngine>();
    }

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 98)
    if (d->engine.use_count() <= 2) {
        KDeclarative::setupEngine(d->engine.get());
    }
#endif

    if (rootContext) {
        d->rootContext = rootContext;
    } else {
        d->rootContext = d->engine->rootContext();
    }

    d->context = new KLocalizedContext(d->rootContext);
    d->rootContext->setContextObject(d->context);
}

QmlObject::~QmlObject()
{
    if (d->engine.use_count() == 1) {
        // QQmlEngine does not take ownership of the QNAM factory so we need to
        // make sure to clean it, but only if we are the last user of the engine
        // otherwise we risk resetting the factory on an engine that is still in
        // use.
        auto factory = d->engine->networkAccessManagerFactory();
        d->engine->setNetworkAccessManagerFactory(nullptr);
        delete factory;
    }

    delete d;
}

void QmlObject::setTranslationDomain(const QString &translationDomain)
{
    d->context->setTranslationDomain(translationDomain);
}

QString QmlObject::translationDomain() const
{
    return d->context->translationDomain();
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

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 98)
void QmlObject::loadPackage(const QString &packageName)
{
    d->package = KPackage::PackageLoader::self()->loadPackage(QStringLiteral("KPackage/GenericQML"));
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
#endif

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
    return d->engine.get();
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

    Q_EMIT q->finished();
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
        Q_EMIT finished();
    }
}

QObject *QmlObject::createObjectFromSource(const QUrl &source, QQmlContext *context, const QVariantHash &initialProperties)
{
    QQmlComponent *component = new QQmlComponent(d->engine.get(), this);
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
        // memory management
        component->setParent(object);
        // reparent to root object if wasn't specified otherwise by initialProperties
        if (!initialProperties.contains(QLatin1String("parent"))) {
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
        return nullptr;
    }
}

}

#include "moc_qmlobject.cpp"
