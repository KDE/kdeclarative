cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.86.0") # handled by release scripts
set(KF_DEP_VERSION "5.86.0") # handled by release scripts
project(KDeclarative VERSION ${KF_VERSION})

# Dependencies
include(FeatureSummary)
find_package(ECM 5.86.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

set(REQUIRED_QT_VERSION 5.15.2)

find_package(Qt5 ${REQUIRED_QT_VERSION} NO_MODULE REQUIRED Qml Quick Gui)
include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(KDEGitCommitHooks)

find_package(KF5Config ${KF_DEP_VERSION} REQUIRED)
find_package(KF5I18n ${KF_DEP_VERSION} REQUIRED)
find_package(KF5IconThemes ${KF_DEP_VERSION} REQUIRED)
find_package(KF5KIO ${KF_DEP_VERSION} REQUIRED)
find_package(KF5WidgetsAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5WindowSystem ${KF_DEP_VERSION} REQUIRED)
find_package(KF5GlobalAccel ${KF_DEP_VERSION} REQUIRED)
find_package(KF5GuiAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Package ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Notifications ${KF_DEP_VERSION} REQUIRED)

#########################################################################

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f02)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055100)

#add_definitions(-Wno-deprecated)

# Includes

include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)
include(ECMAddQch)

set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")

option(BUILD_EXAMPLES "Build and install examples." OFF)
option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")


# ECM setup
ecm_setup_version(
  PROJECT
  VARIABLE_PREFIX KDECLARATIVE
  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kdeclarative_version.h"
  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5DeclarativeConfigVersion.cmake"
  SOVERSION 5)

# Subdirectories
add_definitions(-DTRANSLATION_DOMAIN=\"kdeclarative5\")
ki18n_install(po)

find_package(epoxy)
set_package_properties(epoxy PROPERTIES DESCRIPTION "libepoxy"
                       URL "https://github.com/anholt/libepoxy"
                       TYPE OPTIONAL
                       PURPOSE "OpenGL dispatch library"
                      )

set(HAVE_EPOXY 0)
if(epoxy_FOUND)
    set(HAVE_EPOXY 1)
endif()
add_subdirectory(src)
if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

# Create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Declarative")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5Declarative_QCH
        FILE KF5DeclarativeQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5DeclarativeQchTargets.cmake\")")
endif()

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5DeclarativeConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5DeclarativeConfig.cmake"
  INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5DeclarativeConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5DeclarativeConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT
  KF5DeclarativeTargets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  FILE KF5DeclarativeTargets.cmake
  NAMESPACE KF5::
  COMPONENT Devel
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/kdeclarative_version.h"
  DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF5}"
  COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

if(KF5Config_FOUND AND KF5I18n_FOUND AND KF5IconThemes_FOUND AND KF5KIO_FOUND)
  message("Dependencies for libkdeclarative found: it will be built")
else()
  message("In order to build libkdeclarative you need KF5Config, KF5I18n, KF5IconThemes and KF5KIO")
endif()

if(KF5I18n_FOUND AND KF5WidgetsAddons_FOUND AND KF5WindowSystem_FOUND AND KF5GlobalAccel_FOUND AND KF5KIO_FOUND AND KF5GuiAddons_FOUND)
  message("Dependencies for KQuickControls found: it will be built")
else()
  message("In order to build KQuickControls you need KF5I18n, KF5WidgetsAddons, KF5WindowSystem, KF5GlobalAccel, KF5KIO and KF5GuiAddons")
endif()

if(KF5CoreAddons_FOUND)
  message("Dependencies for KCoreAddons QML bindings found; it will be built")
else()
  message("In order to build KCoreAddons QML bindings you need KCoreAddons")
endif()


kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
