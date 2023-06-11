cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)

set(project_name SiM)

if(NOT DEFINED BUILD_MODE)
    set(BUILD_MODE "DEBUG")
endif()

project(${project_name})
add_compile_options(-Wall -Wextra -Werror -Wpedantic)

if(${BUILD_MODE} STREQUAL "DEBUG")
    add_compile_options(-g -O0)
elseif(${BUILD_MODE} STREQUAL "RELEASE")
    add_compile_options(-O2)
else()
    message(FATAL_ERROR "Unknow build mode")
endif()

if(ENABLE_MEMORY_SANITIZERS)
    add_compile_options(-fsanitize=address -fsanitize=leak -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=leak -fsanitize=undefined)

endif()

if(ENABLE_COVERAGE)
    add_compile_options(-fprofile-arcs -ftest-coverage)
endif()

if(ENABLE_CLANG_TIDY OR ENABLE_CPPCHECK)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set(source
    ${PROJECT_SOURCE_DIR}/src/SampleSource.cpp
)

set(include_locations
    ${PROJECT_SOURCE_DIR}/include
)

set(includes 
    ${PROJECT_SOURCE_DIR}/include/SincQueue.hpp
    ${PROJECT_SOURCE_DIR}/include/SincQueue.inl
)

set(tests_folder ${PROJECT_SOURCE_DIR}/tests)

find_package (Threads)
set(libs 
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(exe ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_custom_target(EXE_TARGET DEPENDS exe)

add_library(${project_name}_lib STATIC ${source})
target_include_directories(${project_name}_lib PUBLIC ${include_locations})
target_link_libraries(exe ${project_name}_lib ${libs})

if(ENABLE_CLANG_TIDY) 
    find_program(CLANG_TIDY "clang-tidy")
    add_custom_target(
        TIDY_TARGET ALL
        COMMAND ${CLANG_TIDY}
        -p ${PROJECT_BINARY_DIR}/compile_commands.json
        --config-file=${PROJECT_SOURCE_DIR}/configs/.clang-tidy
        ${source}
        ${PROJECT_SOURCE_DIR}/src/main.cpp
    )
endif()

if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT "clang-format")
    add_custom_target(
        FORMAT_CHECK_TARGET ALL
        COMMAND clang-format
        --dry-run
        --Werror
        -style=file:${PROJECT_SOURCE_DIR}/configs/.clang-format
        ${includes}
        ${source}
        ${PROJECT_SOURCE_DIR}/src/main.cpp
    )

    add_custom_target(
        FORMAT_TARGET ALL
        COMMAND clang-format
        -i
        --Werror
        -style=file:${PROJECT_SOURCE_DIR}/configs/.clang-format
        ${includes}
        ${source}
        ${PROJECT_SOURCE_DIR}/src/main.cpp
    )
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK "cppcheck")
    add_custom_target(
        CPPCHECK_TARGET ALL
        COMMAND cppcheck
        --enable=all
        --suppress=missingInclude
        --project=${PROJECT_BINARY_DIR}/compile_commands.json
        ${includes}
        ${source}
        ${PROJECT_SOURCE_DIR}/src/main.cpp
    )
endif()

if(ENABLE_COVERAGE OR ENABLE_TESTING OR ENABLE_VALGRIND OR ENABLE_DRD)
    enable_testing()
    add_subdirectory(googletest)
    add_subdirectory(${tests_folder})

    find_program(CTEST "ctest")
    add_custom_target(
        TESTS_TARGET ALL
        COMMAND ctest
        --output-on-failure
        --test-dir ${PROJECT_BINARY_DIR}
        DEPENDS tests_list_target ${libs_names}
    )
endif()

if(ENABLE_COVERAGE)
    target_link_libraries(exe gcov)
endif()
