
set(kdeclarative_SRCS
    qmlobject.cpp
    qmlobject.h
    qmlobjectsharedengine.cpp
    qmlobjectsharedengine.h

)

add_library(KF6Declarative ${kdeclarative_SRCS})
add_library(KF6::Declarative ALIAS KF6Declarative)
ecm_generate_export_header(KF6Declarative
    EXPORT_FILE_NAME ${KDeclarative_BINARY_DIR}/kdeclarative/kdeclarative_export.h
    BASE_NAME KDeclarative
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

# Apps must include <KDeclarative/File> or <kdeclarative/file.h>
target_include_directories(KF6Declarative INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative>")

target_link_libraries(KF6Declarative
  PUBLIC
    KF6::ConfigCore # KCoreConfigSkeleton, in ConfigPropertyMap
    Qt6::Qml
  PRIVATE
    Qt6::Quick  #QQuickImageProvider
    KF6::I18n       #i18n bindings
)
set(KDeclarative_BUILD_INCLUDE_DIRS ${KDeclarative_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(KF6Declarative PUBLIC "$<BUILD_INTERFACE:${KDeclarative_BUILD_INCLUDE_DIRS}>")

set_target_properties(KF6Declarative
  PROPERTIES VERSION ${KDECLARATIVE_VERSION}
  SOVERSION ${KDECLARATIVE_SOVERSION}
  EXPORT_NAME Declarative
)

# Install files
ecm_generate_headers(KDeclarative_CamelCase_HEADERS
  HEADER_NAMES
  QmlObject
  QmlObjectSharedEngine

  PREFIX KDeclarative
  REQUIRED_HEADERS KDeclarative_HEADERS
)
install(FILES ${KDeclarative_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative/KDeclarative COMPONENT Devel)

install(TARGETS KF6Declarative
  EXPORT KF6DeclarativeTargets
  ${KF_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
  ${KDeclarative_BINARY_DIR}/kdeclarative/kdeclarative_export.h
  ${KDeclarative_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative/kdeclarative
  COMPONENT Devel
)

# make available to ecm_add_qch in parent folder
set(KDeclarative_QCH_SOURCES ${KDeclarative_HEADERS} PARENT_SCOPE)
set(KDeclarative_QCH_INCLUDE_DIRS ${KDeclarative_BUILD_INCLUDE_DIRS} PARENT_SCOPE)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KDeclarative LIB_NAME KF6Declarative DEPS "qml" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
