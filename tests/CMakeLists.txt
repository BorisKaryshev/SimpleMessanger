cmake_minimum_required(VERSION 3.9)

add_compile_options(-fprofile-arcs -ftest-coverage)

set(Message_test_name MESSAGE_TESTS)
set(Message_test ${PROJECT_SOURCE_DIR}/tests/Message.cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(${Message_test_name}_tests_run ${Message_test})

set(libs gtest_main gtest gmock gcov ${project_name}_lib Boost::serialization)

target_link_libraries(${Message_test_name}_tests_run ${libs})

add_custom_target(
    tests_list_target 
    DEPENDS 
    ${Message_test_name}_tests_run
)

add_test(NAME ${Message_test_name} COMMAND ${Message_test_name}_tests_run)
