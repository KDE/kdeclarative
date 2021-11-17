set(KF5QuickAddons_LIB_SRCS
    configmodule.cpp
    configmodule.h
    imagetexturescache.cpp
    imagetexturescache.h
    managedconfigmodule.cpp
    managedconfigmodule.h
    managedtexturenode.cpp
    managedtexturenode.h
    qtquicksettings.cpp
    qtquicksettings.h
    quickviewsharedengine.cpp
    quickviewsharedengine.h)
kconfig_add_kcfg_files(KF5QuickAddons_LIB_SRCS renderersettings.kcfgc)

add_library(KF5QuickAddons ${KF5QuickAddons_LIB_SRCS})

ecm_generate_export_header(KF5QuickAddons
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/quickaddons_export.h
    BASE_NAME QuickAddons
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 5.88
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

add_library(KF5::QuickAddons ALIAS KF5QuickAddons)

# Apps must include <KQuickAddons/File> or <kquickaddons/file.h>
target_include_directories(KF5QuickAddons INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative>")

target_link_libraries(KF5QuickAddons
  PUBLIC
    Qt5::Quick
    KF5::CoreAddons
  PRIVATE
    KF5::I18n
    KF5::Declarative
    KF5::ConfigGui
)
if (ANDROID)
    target_link_libraries(KF5QuickAddons PRIVATE GLESv3)
endif()

set_target_properties(KF5QuickAddons
  PROPERTIES VERSION ${KDECLARATIVE_VERSION}
  SOVERSION ${KDECLARATIVE_SOVERSION}
  EXPORT_NAME QuickAddons
)

# Install files
#those are deprecated, offered just for compatibility
ecm_generate_headers(QuickAddons_CamelCase_HEADERS
  HEADER_NAMES
  ImageTexturesCache
  ManagedTextureNode
  PREFIX QuickAddons
  REQUIRED_HEADERS QuickAddons_HEADERS
)
install(FILES ${QuickAddons_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/QuickAddons COMPONENT Devel)

# Install files
ecm_generate_headers(KQuickAddons_CamelCase_HEADERS
  HEADER_NAMES
  ImageTexturesCache
  ManagedTextureNode
  QtQuickSettings
  ConfigModule
  ManagedConfigModule
  QuickViewSharedEngine

  PREFIX KQuickAddons
  REQUIRED_HEADERS KQuickAddons_HEADERS
)
install(FILES ${KQuickAddons_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/KQuickAddons COMPONENT Devel)

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

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/quickaddons_export.h
  ${KQuickAddons_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/kquickaddons
  COMPONENT Devel
)

# make available to ecm_add_qch in parent folder
set(KQuickAddons_QCH_SOURCES ${KQuickAddons_HEADERS} PARENT_SCOPE)
set(KQuickAddons_QCH_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME QuickAddons LIB_NAME KF5QuickAddons DEPS "qml" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/QuickAddons)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
