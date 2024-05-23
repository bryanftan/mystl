cmake_minimum_required(VERSION 3.28)

project(mystl-library)

add_library(${PROJECT_NAME} INTERFACE)
add_library(mystl::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        INTERFACE ${PROJECT_SOURCE_DIR}
)