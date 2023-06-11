cmake_minimum_required(VERSION 3.9)

add_compile_options(-fprofile-arcs -ftest-coverage)

set(SincQueue_test_name SINC_QUEUE_TESTS)
set(SincQueue_test ${PROJECT_SOURCE_DIR}/tests/SincQueue.cpp)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${SincQueue_test_name}_tests_run ${SincQueue_test})

set(libs gtest_main gtest gmock gcov)

target_link_libraries(${SincQueue_test_name}_tests_run ${libs})

add_custom_target(
    tests_list_target 
    DEPENDS 
    ${SincQueue_test_name}_tests_run
)

add_test(NAME ${SincQueue_test_name} COMMAND ${SincQueue_test_name}_tests_run)
