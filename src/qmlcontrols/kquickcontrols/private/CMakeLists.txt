add_library(kquickcontrolsprivate)

generate_export_header(kquickcontrolsprivate)

ecm_add_qml_module(kquickcontrolsprivate URI "org.kde.private.kquickcontrols" VERSION 2.0 GENERATE_PLUGIN_SOURCE)

qt_extract_metatypes(kquickcontrolsprivate)

set_target_properties(kquickcontrolsprivate PROPERTIES
    VERSION     ${PROJECT_VERSION}
    SOVERSION   0
)

target_sources(kquickcontrolsprivate PRIVATE
    keysequencehelper.cpp
    keysequencehelper.h
    keysequencevalidator.cpp
    keysequencevalidator.h
    translationcontext.cpp
    translationcontext.h
    enums.cpp
    enums.h
)

target_link_libraries(kquickcontrolsprivate
    PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    KF6::I18n
    KF6::ConfigGui
    KF6::GuiAddons
)

target_include_directories(kquickcontrolsprivate PRIVATE ${CMAKE_BINARY_DIR})

if (HAVE_KGLOBALACCEL)
    target_link_libraries(kquickcontrolsprivate PRIVATE KF6::GlobalAccel)
endif()

if (NOT ANDROID)
    target_link_libraries(kquickcontrolsprivate PRIVATE KF6::WidgetsAddons)
endif()

ecm_finalize_qml_module(kquickcontrolsprivate)

install(TARGETS kquickcontrolsprivate ${KF_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
