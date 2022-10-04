include(ECMMarkAsTest)

find_package(Qt${QT_MAJOR_VERSION}Test REQUIRED)

add_executable(kdeclarativetest kdeclarativetest.cpp)

target_include_directories(kdeclarativetest
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

ecm_mark_as_test(kdeclarativetest)
target_link_libraries(kdeclarativetest
   Qt${QT_MAJOR_VERSION}::Quick
   Qt${QT_MAJOR_VERSION}::Test
   KF5::I18n
)


add_executable(qimageitemtest qimageitemtest.cpp)

ecm_mark_as_test(qimageitemtest)
target_link_libraries(qimageitemtest
   Qt${QT_MAJOR_VERSION}::Quick
   Qt${QT_MAJOR_VERSION}::Test
)

if (BUILD_EXAMPLES)
    kpackage_install_package(helloworld org.kde.example.helloworld genericqml)
    kpackage_install_package(helloworldnowindow org.kde.example.helloworldnowindow genericqml)
endif()

