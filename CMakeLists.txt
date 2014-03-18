cmake_minimum_required(VERSION 2.8.12)

project(KQuickControls)

# Make CPack available to easy generate binary packages
include(CPack)
include(FeatureSummary)
include(GenerateExportHeader)

################# set KDE specific information #################

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(ECMGenerateHeaders)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

################# Enable C++0x (still too early for -std=c++11) features for clang and gcc #################

if(UNIX)
   #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
   add_definitions("-Wall -std=c++0x")
endif()

################# now find all used packages #################

set (QT_MIN_VERSION "5.2.0")
set(KF5_VERSION "4.97.0")

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Quick Qml Gui)

find_package(KF5IconThemes ${KF5_VERSION})

find_package(KF5I18n ${KF5_VERSION})
find_package(KF5WidgetsAddons ${KF5_VERSION})
find_package(KF5WindowSystem ${KF5_VERSION})
find_package(KF5GlobalAccel ${KF5_VERSION})

find_package(KF5KIO ${KF5_VERSION})
find_package(KF5GuiAddons ${KF5_VERSION})


#########################################################################

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)

#add_definitions(-Wno-deprecated)
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS -DQT_NO_CAST_FROM_BYTEARRAY -DQT_NO_KEYWORDS)



################# list the subdirectories #################
add_subdirectory(autotests)
add_subdirectory(src)


feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
