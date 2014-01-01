
set(kdeclarative_SRCS
  configpropertymap.cpp
  qmlobject.cpp
  kdeclarative.cpp
  private/rootcontext.cpp
  private/kiconprovider.cpp
  private/kioaccessmanagerfactory.cpp
  private/qmlobject_p.h
)

add_library(KF5Declarative ${kdeclarative_SRCS})
generate_export_header(KF5Declarative EXPORT_FILE_NAME ${KDeclarative_BINARY_DIR}/kdeclarative/kdeclarative_export.h BASE_NAME KDeclarative)
add_library(KF5::Declarative ALIAS KF5Declarative)

# Apps must include <KDeclarative/File> or <kdeclarative/file.h>
target_include_directories(KF5Declarative INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>")

target_link_libraries(KF5Declarative
  PUBLIC
    Qt5::Qml
  PRIVATE
    Qt5::Quick  #QQuickImageProvider
    KF5::ConfigCore # KCoreConfigSkeleton, in ConfigPropertyMap
    KF5::I18n       #i18n bindings
    KF5::KIOWidgets     #KIO::AccessManager
    KF5::IconThemes #KIconProvider
)
target_include_directories(KF5Declarative PUBLIC "$<BUILD_INTERFACE:${KDeclarative_BINARY_DIR}>")

set_target_properties(KF5Declarative
  PROPERTIES VERSION ${KDECLARATIVE_VERSION_STRING}
  SOVERSION ${KDECLARATIVE_SOVERSION}
  EXPORT_NAME Declarative
)

# Install files
ecm_generate_headers(
  KDeclarative
  QmlObject
  ConfigPropertyMap

  MODULE_NAME KDeclarative
  REQUIRED_HEADERS KDeclarative_HEADERS
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/KDeclarative DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel)

install(TARGETS KF5Declarative
  EXPORT KF5DeclarativeTargets
  ${INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
  ${KDeclarative_BINARY_DIR}/kdeclarative/kdeclarative_export.h
  ${KDeclarative_HEADERS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/kdeclarative
  COMPONENT Devel
)
