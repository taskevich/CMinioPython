cmake_minimum_required(VERSION 3.10)
project(sync CXX)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(miniocpp REQUIRED)

add_library(sync MODULE
    sync.cpp
    ../minio_api.cpp
)

target_include_directories(sync PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(sync PRIVATE ${Python3_LIBRARIES})
target_link_libraries(sync PRIVATE miniocpp::miniocpp)

set_target_properties(sync PROPERTIES
    PREFIX ""
    SUFFIX ".pyd"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cminiopy
)
