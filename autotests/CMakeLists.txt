INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/..")

include(ECMAddTests)

find_package(Qt5Test REQUIRED)

ecm_add_test(columnproxymodeltest.cpp
    ../src/qmlcontrols/kquickcontrolsaddons/columnproxymodel.cpp
    TEST_NAME fullmodelaccesstest
    LINK_LIBRARIES Qt5::Gui Qt5::Test)

ecm_add_test(quickviewsharedengine.cpp
    util.cpp
    TEST_NAME quickviewsharedengine
    LINK_LIBRARIES Qt5::Quick KF5::QuickAddons Qt5::Test)


