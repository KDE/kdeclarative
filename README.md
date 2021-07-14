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
the application and to get some information about the platform,
that influences the behavior of the QML components.

Full documentation in KDeclarative::KDeclarative

### KDeclarative::QmlObject

An object that instantiates an entire QML context, with its own declarative engine.

Full documentation in KDeclarative::QmlObject

### KDeclarative::ConfigPropertyMap

An object that (optionally) automatically saves changes in a
property map to a configuration object (e.g. a KConfig file).

Full documentation in KDeclarative::ConfigPropertyMap
