cmake_minimum_required(VERSION 3.9)

add_compile_options(-fprofile-arcs -ftest-coverage)

set(Executor_test_name EXECUTOR_TESTS)
set(Executor_test ${PROJECT_SOURCE_DIR}/tests/Executor.cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(${Executor_test_name}_tests_run ${Executor_test})

set(libs gtest_main gtest gmock gcov ${project_name}_lib)

target_link_libraries(${Executor_test_name}_tests_run ${libs})

add_custom_target(
    tests_list_target 
    DEPENDS 
    ${Executor_test_name}_tests_run
)

add_test(NAME ${Executor_test_name} COMMAND ${Executor_test_name}_tests_run)
