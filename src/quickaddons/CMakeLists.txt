set(KF6QuickAddons_LIB_SRCS
    configmodule.cpp
    configmodule.h
    managedconfigmodule.cpp
    managedconfigmodule.h
    quickviewsharedengine.cpp
    quickviewsharedengine.h)
kconfig_add_kcfg_files(KF6QuickAddons_LIB_SRCS renderersettings.kcfgc)

add_library(KF6QuickAddons ${KF6QuickAddons_LIB_SRCS})

ecm_generate_export_header(KF6QuickAddons
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/quickaddons_export.h
    BASE_NAME QuickAddons
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

add_library(KF6::QuickAddons ALIAS KF6QuickAddons)

# Apps must include <KQuickAddons/File> or <kquickaddons/file.h>
target_include_directories(KF6QuickAddons INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative>"
                                          PUBLIC "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>" # for kdeclarative/kdeclarative_export.h
)

target_link_libraries(KF6QuickAddons
  PUBLIC
    Qt6::Quick
    KF6::CoreAddons
  PRIVATE
    KF6::I18n
    KF6::Declarative
    KF6::ConfigGui
    KF6::Package
)
if (ANDROID)
    target_link_libraries(KF6QuickAddons PRIVATE GLESv3)
endif()

set_target_properties(KF6QuickAddons
  PROPERTIES VERSION ${KDECLARATIVE_VERSION}
  SOVERSION ${KDECLARATIVE_SOVERSION}
  EXPORT_NAME QuickAddons
)

# Install files
ecm_generate_headers(KQuickAddons_CamelCase_HEADERS
  HEADER_NAMES
  ConfigModule
  ManagedConfigModule
  QuickViewSharedEngine

  PREFIX KQuickAddons
  REQUIRED_HEADERS KQuickAddons_HEADERS
)
install(FILES ${KQuickAddons_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative/KQuickAddons COMPONENT Devel)

install(TARGETS KF6QuickAddons
  EXPORT KF6DeclarativeTargets
  ${KF_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/quickaddons_export.h
  ${KQuickAddons_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative/kquickaddons
  COMPONENT Devel
)

# make available to ecm_add_qch in parent folder
set(KQuickAddons_QCH_SOURCES ${KQuickAddons_HEADERS} PARENT_SCOPE)
set(KQuickAddons_QCH_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME QuickAddons LIB_NAME KF6QuickAddons DEPS "qml" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/QuickAddons)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
