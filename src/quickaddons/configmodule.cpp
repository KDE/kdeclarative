/*
    This file is part of the KDE libraries

    SPDX-FileCopyrightText: 1999 Matthias Hoelzer-Kluepfel <hoelzer@kde.org>
    SPDX-FileCopyrightText: 2001 Michael Goffioul <kdeprint@swing.be>
    SPDX-FileCopyrightText: 2004 Frans Englich <frans.englich@telia.com>
    SPDX-FileCopyrightText: 2009 Dario Freddi <drf@kde.org>
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configmodule.h"

#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlFileSelector>
#include <QQuickItem>
#include <QUrl>

#include <KAboutData>
#include <KLocalizedString>
#include <kdeclarative/qmlobject.h>
#include <kdeclarative/qmlobjectsharedengine.h>

#include <KPackage/Package>
#include <KPackage/PackageLoader>

namespace KQuickAddons
{
class ConfigModulePrivate
{
public:
    ConfigModulePrivate(ConfigModule *module)
        : _q(module)
        , _qmlObject(nullptr)
        , _buttons(ConfigModule::Help | ConfigModule::Default | ConfigModule::Apply)
        , _useRootOnlyMessage(false)
        , _needsAuthorization(false)
        , _needsSave(false)
        , _representsDefaults(false)
        , _defaultsIndicatorVisible(false)
    {
    }

    void authStatusChanged(int status);
    QString componentName() const;

    ConfigModule *_q;
    KDeclarative::QmlObject *_qmlObject;
    ConfigModule::Buttons _buttons;
    std::unique_ptr<const KAboutData> _about;
    KPluginMetaData _metaData;
    QString _rootOnlyMessage;
    QString _quickHelp;
    QString _errorString;
    QList<QQuickItem *> subPages;
    int _columnWidth = -1;
    int currentIndex = 0;
    bool _useRootOnlyMessage : 1;

    bool _needsAuthorization : 1;
    bool _needsSave : 1;
    bool _representsDefaults : 1;
    bool _defaultsIndicatorVisible : 1;
    QString _authActionName;

    static QHash<QObject *, ConfigModule *> s_rootObjects;
};

QHash<QObject *, ConfigModule *> ConfigModulePrivate::s_rootObjects = QHash<QObject *, ConfigModule *>();

QString ConfigModulePrivate::componentName() const
{
    if (_about) {
        return _about->componentName();
    } else {
        return _metaData.pluginId();
    }
}

#if QUICKADDONS_BUILD_DEPRECATED_SINCE(5, 88)
ConfigModule::ConfigModule(const KAboutData *aboutData, QObject *parent, const QVariantList &)
    : QObject(parent)
    , d(new ConfigModulePrivate(this))
{
    setAboutData(aboutData);
}
#endif

#if QUICKADDONS_BUILD_DEPRECATED_SINCE(5, 88)
ConfigModule::ConfigModule(const KPluginMetaData &metaData, QObject *parent, const QVariantList &)
    : QObject(parent)
    , d(new ConfigModulePrivate(this))
{
    KAboutData *aboutData =
        new KAboutData(metaData.pluginId(), metaData.name(), metaData.version(), metaData.description(), KAboutLicense::byKeyword(metaData.license()).key());

    const auto authors = metaData.authors();
    for (auto &author : authors) {
        aboutData->addAuthor(author.name(), author.task(), author.emailAddress(), author.webAddress(), author.ocsUsername());
    }
    setAboutData(aboutData);
}
#endif

ConfigModule::ConfigModule(QObject *parent, const QVariantList &)
    : QObject(parent)
    , d(new ConfigModulePrivate(this))
{
}

ConfigModule::ConfigModule(QObject *parent, const KPluginMetaData &metaData, const QVariantList &)
    : QObject(parent)
    , d(new ConfigModulePrivate(this))
{
    d->_metaData = metaData;
}

ConfigModule::~ConfigModule()
{
    // in case mainUi was never called
    if (d->_qmlObject) {
        ConfigModulePrivate::s_rootObjects.remove(d->_qmlObject->rootContext());
    }

    delete d->_qmlObject;
    delete d;
}

ConfigModule *ConfigModule::qmlAttachedProperties(QObject *object)
{
    // at the moment of the attached object creation, the root item is the only one that hasn't a parent
    // only way to avoid creation of this attached for everybody but the root item
    const QQmlEngine *engine = qmlEngine(object);
    QQmlContext *cont = QQmlEngine::contextForObject(object);

    // Search the qml context that is the "root" for the sharedqmlobject, which
    // is an ancestor of QQmlEngine::contextForObject(object) and the direct child
    // of the engine's root context: we can do this assumption on the internals as
    // we are distributed on the same repo.
    while (cont->parentContext() && cont->parentContext() != engine->rootContext()) {
        cont = cont->parentContext();
    }

    if (!object->parent() && ConfigModulePrivate::s_rootObjects.contains(cont)) {
        return ConfigModulePrivate::s_rootObjects.value(cont);
    } else {
        return nullptr;
    }
}

QQuickItem *ConfigModule::mainUi()
{
    if (d->_qmlObject) {
        return qobject_cast<QQuickItem *>(d->_qmlObject->rootObject());
    }

    d->_errorString.clear();

    // if we have a qml context, hook up to it and use its engine
    // this ensure that in e.g. Plasma config dialogs that use a different engine
    // so they can have different QtQuick Controls styles, we don't end up using
    // the shared engine that is used by the rest of plasma

    QQmlContext *ctx = QQmlEngine::contextForObject(this);

    if (ctx && ctx->engine()) {
        d->_qmlObject = new KDeclarative::QmlObject(ctx->engine(), ctx, this);
    } else {
        d->_qmlObject = new KDeclarative::QmlObjectSharedEngine(this);
    }

    ConfigModulePrivate::s_rootObjects[d->_qmlObject->rootContext()] = this;
    d->_qmlObject->setTranslationDomain(d->componentName());
    d->_qmlObject->setInitializationDelayed(true);

    KPackage::Package package = KPackage::PackageLoader::self()->loadPackage(QStringLiteral("KPackage/GenericQML"));
    package.setDefaultPackageRoot(QStringLiteral("kpackage/kcms"));
    package.setPath(d->componentName());
    if (d->_metaData.isValid() && !package.metadata().isValid()) {
        package.setMetadata(d->_metaData);
    }

    if (!package.isValid()) {
        d->_errorString = i18n("Invalid KPackage '%1'", d->componentName());
        qWarning() << "Error loading the module" << d->componentName() << ": invalid KPackage";
        return nullptr;
    }

    if (package.filePath("mainscript").isEmpty()) {
        d->_errorString = i18n("No QML file provided");
        qWarning() << "Error loading the module" << d->componentName() << ": no QML file provided";
        return nullptr;
    }

    new QQmlFileSelector(d->_qmlObject->engine(), d->_qmlObject->engine());
    d->_qmlObject->setSource(package.fileUrl("mainscript"));
    d->_qmlObject->rootContext()->setContextProperty(QStringLiteral("kcm"), this);
    d->_qmlObject->completeInitialization();

    if (d->_qmlObject->status() != QQmlComponent::Ready) {
        d->_errorString = d->_qmlObject->mainComponent()->errorString();
        return nullptr;
    }

    return qobject_cast<QQuickItem *>(d->_qmlObject->rootObject());
}

void ConfigModule::push(const QString &fileName, const QVariantMap &propertyMap)
{
    // ensure main ui is created
    if (!mainUi()) {
        return;
    }

    // TODO: package as member
    KPackage::Package package = KPackage::PackageLoader::self()->loadPackage(QStringLiteral("KPackage/GenericQML"));
    package.setDefaultPackageRoot(QStringLiteral("kpackage/kcms"));
    package.setPath(d->componentName());

    QVariantHash propertyHash;
    for (auto it = propertyMap.begin(), end = propertyMap.end(); it != end; ++it) {
        propertyHash.insert(it.key(), it.value());
    }

    QObject *object = d->_qmlObject->createObjectFromSource(QUrl::fromLocalFile(package.filePath("ui", fileName)), d->_qmlObject->rootContext(), propertyHash);

    QQuickItem *item = qobject_cast<QQuickItem *>(object);
    if (!item) {
        object->deleteLater();
        return;
    }

    d->subPages << item;
    Q_EMIT pagePushed(item);
    Q_EMIT depthChanged(depth());
    setCurrentIndex(d->currentIndex + 1);
}

void ConfigModule::push(QQuickItem *item)
{
    // ensure main ui is created
    if (!mainUi()) {
        return;
    }

    d->subPages << item;
    Q_EMIT pagePushed(item);
    Q_EMIT depthChanged(depth());
    setCurrentIndex(d->currentIndex + 1);
}

void ConfigModule::pop()
{
    if (QQuickItem *page = takeLast()) {
        page->deleteLater();
    }
}

QQuickItem *ConfigModule::takeLast()
{
    if (d->subPages.isEmpty()) {
        return nullptr;
    }
    QQuickItem *page = d->subPages.takeLast();
    Q_EMIT pageRemoved();
    Q_EMIT depthChanged(depth());
    setCurrentIndex(qMin(d->currentIndex, depth() - 1));
    return page;
}

void ConfigModule::showPassiveNotification(const QString &message, const QVariant &timeout, const QString &actionText, const QJSValue &callBack)
{
    Q_EMIT passiveNotificationRequested(message, timeout, actionText, callBack);
}

ConfigModule::Buttons ConfigModule::buttons() const
{
    return d->_buttons;
}

void ConfigModule::setButtons(const KQuickAddons::ConfigModule::Buttons buttons)
{
    if (d->_buttons == buttons) {
        return;
    }

    d->_buttons = buttons;
    Q_EMIT buttonsChanged();
}

void ConfigModule::setNeedsAuthorization(bool needsAuth)
{
    if (d->_needsAuthorization == needsAuth) {
        return;
    }

    d->_needsAuthorization = needsAuth;
    if (needsAuth) {
        d->_authActionName = QLatin1String("org.kde.kcontrol.") + d->componentName() + QLatin1String(".save");
        d->_needsAuthorization = true;

    } else {
        d->_authActionName = QString();
    }

    Q_EMIT needsAuthorizationChanged();
    Q_EMIT authActionNameChanged();
}

bool ConfigModule::needsAuthorization() const
{
    return d->_needsAuthorization;
}

QString ConfigModule::name() const
{
    if (d->_metaData.isValid()) {
        return d->_metaData.name();
    }
    return d->_about->displayName();
}

QString ConfigModule::description() const
{
    if (d->_metaData.isValid()) {
        return d->_metaData.description();
    }
    return d->_about->shortDescription();
}

int ConfigModule::columnWidth() const
{
    return d->_columnWidth;
}

void ConfigModule::setColumnWidth(int width)
{
    if (d->_columnWidth == width) {
        return;
    }

    d->_columnWidth = width;
    Q_EMIT columnWidthChanged(width);
}

int ConfigModule::depth() const
{
    return d->subPages.count() + 1;
}

void ConfigModule::setCurrentIndex(int index)
{
    if (index < 0 || index > d->subPages.count() || index == d->currentIndex) {
        return;
    }

    d->currentIndex = index;

    Q_EMIT currentIndexChanged(index);
}

int ConfigModule::currentIndex() const
{
    return d->currentIndex;
}

void ConfigModule::setAuthActionName(const QString &name)
{
    if (d->_authActionName == name) {
        return;
    }

    d->_authActionName = name;
    d->_needsAuthorization = true;

    Q_EMIT needsAuthorizationChanged();
    Q_EMIT authActionNameChanged();
}

QString ConfigModule::authActionName() const
{
    return d->_authActionName;
}

QQmlEngine *ConfigModule::engine() const
{
    return d->_qmlObject->engine();
}

QQmlComponent::Status ConfigModule::status() const
{
    if (!d->_qmlObject) {
        return QQmlComponent::Null;
    }

    return d->_qmlObject->status();
}

QString ConfigModule::errorString() const
{
    return d->_errorString;
}

void ConfigModule::load()
{
    setNeedsSave(false);
}

void ConfigModule::save()
{
    setNeedsSave(false);
}

void ConfigModule::defaults()
{
}

#if QUICKADDONS_BUILD_DEPRECATED_SINCE(5, 88)
const KAboutData *ConfigModule::aboutData() const
{
    // If the ConfigModule was created from a KPluginMetaData lazily create a KAboutData from it
    if (d->_metaData.isValid() && !d->_about) {
        KAboutData *aboutData = new KAboutData(d->_metaData.pluginId(),
                                               d->_metaData.name(),
                                               d->_metaData.version(),
                                               d->_metaData.description(),
                                               KAboutLicense::byKeyword(d->_metaData.license()).key());

        const auto authors = d->_metaData.authors();
        for (auto &author : authors) {
            aboutData->addAuthor(author.name(), author.task(), author.emailAddress(), author.webAddress(), author.ocsUsername());
        }

        d->_about.reset(aboutData);
    }

    return d->_about.get();
}
#endif

void ConfigModule::setAboutData(const KAboutData *about)
{
    d->_about.reset(about);
}

void ConfigModule::setRootOnlyMessage(const QString &message)
{
    if (d->_rootOnlyMessage == message) {
        return;
    }

    d->_rootOnlyMessage = message;
    Q_EMIT rootOnlyMessageChanged();
}

QString ConfigModule::rootOnlyMessage() const
{
    return d->_rootOnlyMessage;
}

void ConfigModule::setUseRootOnlyMessage(bool on)
{
    if (d->_useRootOnlyMessage == on) {
        return;
    }

    d->_useRootOnlyMessage = on;

    Q_EMIT useRootOnlyMessageChanged();
}

bool ConfigModule::useRootOnlyMessage() const
{
    return d->_useRootOnlyMessage;
}

QQuickItem *ConfigModule::subPage(int index) const
{
    return d->subPages[index];
}

void ConfigModule::setQuickHelp(const QString &help)
{
    if (d->_quickHelp == help) {
        return;
    }

    d->_quickHelp = help;

    Q_EMIT quickHelpChanged();
}

QString ConfigModule::quickHelp() const
{
    return d->_quickHelp;
}

void ConfigModule::setNeedsSave(bool needs)
{
    if (needs == d->_needsSave) {
        return;
    }

    d->_needsSave = needs;
    Q_EMIT needsSaveChanged();
}

bool ConfigModule::needsSave()
{
    return d->_needsSave;
}

void ConfigModule::setRepresentsDefaults(bool defaults)
{
    if (defaults == d->_representsDefaults) {
        return;
    }

    d->_representsDefaults = defaults;
    Q_EMIT representsDefaultsChanged();
}

bool ConfigModule::representsDefaults()
{
    return d->_representsDefaults;
}

bool ConfigModule::defaultsIndicatorsVisible() const
{
    return d->_defaultsIndicatorVisible;
}

void ConfigModule::setDefaultsIndicatorsVisible(bool visible)
{
    if (d->_defaultsIndicatorVisible == visible) {
        return;
    }
    d->_defaultsIndicatorVisible = visible;
    Q_EMIT defaultsIndicatorsVisibleChanged();
}
}

#include "moc_configmodule.cpp"
