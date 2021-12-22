INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/..")

include(ECMAddTests)

find_package(Qt${QT_MAJOR_VERSION}Test REQUIRED)

ecm_add_test(columnproxymodeltest.cpp
    ../src/qmlcontrols/kquickcontrolsaddons/columnproxymodel.cpp
    TEST_NAME fullmodelaccesstest
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Gui Qt${QT_MAJOR_VERSION}::Test)

ecm_add_test(quickviewsharedengine.cpp
    util.cpp
    TEST_NAME quickviewsharedengine
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Quick KF5::QuickAddons Qt${QT_MAJOR_VERSION}::Test)


