# KDeclarative

Integration of QML and KDE work spaces

## Introduction

KDeclarative provides integration of QML and KDE work spaces.
It's comprises two parts: a library used by the C++ part of your application
to intergrate QML with KDE Frameworks specific features, and a series of
QML imports that offer bindings to some of the Frameworks.

## KDeclarative library

KDeclarative exposes 3 namespaced classes: KDeclarative::KDeclarative, KDeclarative::QmlObject, KDeclarative::ConfigPropertyMap

### KDeclarative::KDeclarative

The KDeclarative class is used to manipulate the QQmlEngine instance used by 
the application and to get some informations about the platform, 
that influences the behavior of the QML components.

In order to use it, create an instance of KDeclarative. You will need a 
pointer to a QQmlEngine, and call **both** `setDeclarativeEngine(engine)` and
`setupContext()` on your instance. You need to call `setupEngine(engine)`
at least once on the engine as well.

    KDeclarative::setupEngine(engine);  // if not done elsewhere
    KDeclarative::KDeclarative decl;
    decl.setDeclarativeEngine(engine);
    decl.setupContext();
    
This will add the following things to the engine:
* Use a KIOAccessManagerFactory instead of the stock QQmlNetworkAccessManagerFactory
* Add a QML icon provider, that makes possible for the Image {} element to load images using the scheme "image:/"
* Use the given engine for this context.
* Set a new rootContextObject() that exposes all the i18n() functions from the KI18n framework.
  They will be available in the global QML context: just call i18n() from anywhere in your QML code.

### KDeclarative::QmlObject



### KDeclarative::ConfigPropertyMap

