INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/..")

include(ECMAddTests)

find_package(Qt6Test REQUIRED)

ecm_add_test(columnproxymodeltest.cpp
    ../src/qmlcontrols/kquickcontrolsaddons/columnproxymodel.cpp
    TEST_NAME fullmodelaccesstest
    LINK_LIBRARIES Qt6::Gui Qt6::Test)

