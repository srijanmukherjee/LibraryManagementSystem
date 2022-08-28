cmake_minimum_required(VERSION 3.10)

project(AppTest VERSION 1.0)

set(LinkLibraries gtest_main AppLib)
add_executable(ModelTest ModelTest.cpp)
target_link_libraries(ModelTest PUBLIC ${LinkLibraries})

add_test(
    NAME ModelTest
    COMMAND ModelTest
)