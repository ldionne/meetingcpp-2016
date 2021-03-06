# Copyright Louis Dionne 2016
# Distributed under the Boost Software License, Version 1.0.

cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#=============================================================================
# Setup required packages
#=============================================================================
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include(ExternalProject)
ExternalProject_Add(install-Hana EXCLUDE_FROM_ALL 1
    URL https://github.com/boostorg/hana/archive/develop.zip
    TIMEOUT 120
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dependencies/hana"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
)
ExternalProject_Get_Property(install-Hana SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

include(ExternalProject)
ExternalProject_Add(install-Brigand EXCLUDE_FROM_ALL 1
    URL https://github.com/edouarda/brigand/archive/master.zip
    TIMEOUT 120
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dependencies/brigand"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
)
ExternalProject_Get_Property(install-Brigand SOURCE_DIR)
include_directories(${SOURCE_DIR})

include(ExternalProject)
ExternalProject_Add(install-Metal EXCLUDE_FROM_ALL 1
    URL https://github.com/brunocodutra/metal/archive/master.zip
    TIMEOUT 120
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dependencies/metal"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
)
ExternalProject_Get_Property(install-Metal SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

add_custom_target(install-dependencies DEPENDS install-Hana install-Brigand install-Metal)


#=============================================================================
# Setup compiler flags
#=============================================================================
include(CheckCXXCompilerFlag)
macro(append_cxx_flag testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        add_compile_options(${flag})
    endif()
endmacro()

append_cxx_flag(HAS_W_FLAG                         -W)
append_cxx_flag(HAS_WALL_FLAG                      -Wall)
append_cxx_flag(HAS_WEXTRA_FLAG                    -Wextra)
append_cxx_flag(HAS_WNO_LONG_LONG_FLAG             -Wno-long-long)
append_cxx_flag(HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG -Wno-unused-local-typedefs)
append_cxx_flag(HAS_WNO_UNUSED_PARAMETER_FLAG      -Wno-unused-parameter)
append_cxx_flag(HAS_WWRITE_STRINGS_FLAG            -Wwrite-strings)
append_cxx_flag(HAS_STDCXX1Y_FLAG                  -std=c++1z)
append_cxx_flag(HAS_PEDANTIC_FLAG                  -pedantic)
append_cxx_flag(HAS_WNO_GNU_STRING_UDL             -Wno-gnu-string-literal-operator-template)
append_cxx_flag(HAS_WNO_UNUSED_VARIABLE            -Wno-unused-variable)


#=============================================================================
# Setup code samples
#=============================================================================
enable_testing()
add_custom_target(samples)

file(GLOB CODE_SAMPLES code/*.cpp)
foreach(_file IN LISTS CODE_SAMPLES)
    file(RELATIVE_PATH _target ${CMAKE_CURRENT_SOURCE_DIR}/code ${_file})
    string(REPLACE ".cpp" "" _target "${_target}")
    add_executable(sample.${_target} "${_file}")
    target_compile_options(sample.${_target} PRIVATE -O3)
    add_dependencies(samples sample.${_target})
    add_test(sample.${_target} sample.${_target})
endforeach()

add_custom_target(check ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS samples
    COMMENT "Build all the samples and then run them.")


#=============================================================================
# Setup benchmarks
#=============================================================================
include(metabench)
add_custom_target(benchmarks)

foreach(target hana hana.runtime std.function std.unordered_map)
    metabench_add_dataset(benchmark.callbacks.${target}
        benchmark/callbacks.${target}.cpp.erb
        "[1, 2, 4, 6, 8, 10]"
        NAME ${target}
        ENV "{maxn: 10, iterations: 10_000_000}")
    target_compile_options(benchmark.callbacks.${target} PRIVATE -O3 -flto)
endforeach()

metabench_add_chart(benchmark.callbacks
    DATASETS benchmark.callbacks.hana
             benchmark.callbacks.std.function
             benchmark.callbacks.std.unordered_map
    ASPECT EXECUTION_TIME
    XLABEL "Number of events triggered (x 10M)"
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/callbacks.html)

metabench_add_chart(benchmark.callbacks.runtime
    DATASETS benchmark.callbacks.hana.runtime
             benchmark.callbacks.std.unordered_map
    ASPECT EXECUTION_TIME
    XLABEL "Number of events triggered (x 10M)"
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/callbacks.runtime.html)

add_dependencies(benchmarks benchmark.callbacks benchmark.callbacks.runtime)
