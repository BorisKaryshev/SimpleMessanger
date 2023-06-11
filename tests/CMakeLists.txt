cmake_minimum_required(VERSION 3.9)

add_compile_options(-fprofile-arcs -ftest-coverage)

set(Sample_test_name SAMPLE_TESTS)
set(Sample_test ${PROJECT_SOURCE_DIR}/tests/AnimeFramework.cpp)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${Sample_test_name}_tests_run ${Sample_test})

set(libs gtest_main gtest gmock gcov)

target_link_libraries(${Sample_test_name}_tests_run ${libs})

add_custom_target(
    tests_list_target 
    DEPENDS 
    ${Sample_test_name}_tests_run
)

add_test(NAME ${Sample_test_name} COMMAND ${Sample_test_name}_tests_run)
