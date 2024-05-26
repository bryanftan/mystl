cmake_minimum_required(VERSION 3.28)
project(mystl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_library(mystl INTERFACE)
add_library(mystl::lib ALIAS mystl)
target_include_directories(mystl
        INTERFACE ${PROJECT_SOURCE_DIR}/include
)

enable_testing()

option(BUILD_TEST "Build Test" ON)

if (BUILD_TEST)
    add_subdirectory(test)
endif ()
