cmake_minimum_required(VERSION 3.9)

add_compile_options(-fprofile-arcs -ftest-coverage)

set(CMAKE_CXX_STANDARD 20)
set(libs gtest_main gtest gmock gcov ${project_name}_lib Boost::serialization)

set(Executor_test_name EXECUTOR_TESTS)
set(Executor_test ${PROJECT_SOURCE_DIR}/tests/Executor.cpp)
add_executable(${Executor_test_name}_tests_run ${Executor_test})
target_link_libraries(${Executor_test_name}_tests_run ${libs})

set(SincQueue_test_name SINC_QUEUE_TESTS)
set(SincQueue_test ${PROJECT_SOURCE_DIR}/tests/SincQueue.cpp)
add_executable(${SincQueue_test_name}_tests_run ${SincQueue_test})
target_link_libraries(${SincQueue_test_name}_tests_run ${libs})

set(Message_test_name MESSAGE_TESTS)
set(Message_test ${PROJECT_SOURCE_DIR}/tests/Message.cpp)
add_executable(${Message_test_name}_tests_run ${Message_test})
target_link_libraries(${Message_test_name}_tests_run ${libs})

add_custom_target(
    tests_list_target 
    DEPENDS 
    ${Message_test_name}_tests_run
    ${Executor_test_name}_tests_run
    ${SincQueue_test_name}_tests_run
)

add_test(NAME ${Message_test_name} COMMAND ${Message_test_name}_tests_run)
add_test(NAME ${Executor_test_name} COMMAND ${Executor_test_name}_tests_run)
add_test(NAME ${SincQueue_test_name} COMMAND ${SincQueue_test_name}_tests_run)
