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

The KDeclarative class is used to manipulate the QQmlEngine instance used by the application and to get some informations about the platform, that influences the behavior of the QML components.

In order to use it, create an instance of KDeclarative, and as soon as you have a pointer to your QQmlEngine, call setupBindings:

    KDeclarative::KDeclarative decl;
    decl.setupBindings(engine);

This will add the following things to the engine:
* use a KIOAccessManagerFactory instead of the stock QQmlNetworkAccessManagerFactory
* set a new rootContextObject() that exposes all the i18n() functions from the KI18n framework.
  they will be available in the global QML context: just call i18n() from anywhere in your QML code.
* Add a QML icon provider, that makes possible for the Image {} element to load stuff from the scheme "image:/"

### KDeclarative::QmlObject



### KDeclarative::ConfigPropertyMap

