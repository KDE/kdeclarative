include(ECMMarkAsTest)

find_package(Qt${QT_MAJOR_VERSION}Test REQUIRED)

add_executable(qimageitemtest qimageitemtest.cpp)

ecm_mark_as_test(qimageitemtest)
target_link_libraries(qimageitemtest
   Qt${QT_MAJOR_VERSION}::Quick
   Qt${QT_MAJOR_VERSION}::Test
)
