add_library(KF5QuickAddons
            imagetexturescache.cpp
            managedtexturenode.cpp
            configmodule.cpp)
generate_export_header(KF5QuickAddons EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/quickaddons_export.h BASE_NAME QuickAddons)
add_library(KF5::QuickAddons ALIAS KF5QuickAddons)

# Apps must include <QuickAddons/File> or <quickaddons/file.h>
target_include_directories(KF5QuickAddons INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative>")

target_link_libraries(KF5QuickAddons
  PUBLIC
    Qt5::Qml
  PRIVATE
    Qt5::Quick  #QQuickImageProvider
    KF5::ConfigCore # KCoreConfigSkeleton, in ConfigPropertyMap
    KF5::I18n       #i18n bindings
    KF5::KIOWidgets     #KIO::AccessManager
    KF5::IconThemes #KIconProvider
    KF5::Declarative #QmlObject
    KF5::Auth
)
target_include_directories(KF5QuickAddons PUBLIC "$<BUILD_INTERFACE:${QuickAddons_BINARY_DIR}/../;${CMAKE_CURRENT_BINARY_DIR}/../>")

set_target_properties(KF5QuickAddons
  PROPERTIES VERSION ${KDECLARATIVE_VERSION_STRING}
  SOVERSION ${KDECLARATIVE_SOVERSION}
  EXPORT_NAME QuickAddons
)

# Install files
ecm_generate_headers(QuickAddons_CamelCase_HEADERS
  HEADER_NAMES
  ImageTexturesCache
  ManagedTextureNode
  ConfigModule

  PREFIX QuickAddons
  REQUIRED_HEADERS QuickAddons_HEADERS
)
install(FILES ${QuickAddons_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/QuickAddons COMPONENT Devel)

install(TARGETS KF5QuickAddons
  EXPORT KF5DeclarativeTargets
  ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/quickaddons_export.h
  ${QuickAddons_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/quickaddons
  COMPONENT Devel
)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME QuickAddons LIB_NAME KF5QuickAddons DEPS "qml" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/QuickAddons)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
