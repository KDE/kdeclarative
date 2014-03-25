INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/..")

include(ECMMarkAsTest)

find_package(Qt5Test REQUIRED)

add_executable(fullmodelaccesstest
    columnproxymodeltest.cpp
    ../src/qmlcontrols/kquickcontrolsaddons/columnproxymodel.cpp
    modeltest.cpp
)
target_link_libraries(fullmodelaccesstest
    Qt5::Gui
    Qt5::Test
    )

add_test(fullmodelaccesstest fullmodelaccesstest)
ecm_mark_as_test(fullmodelaccesstest)
