cmake_minimum_required(VERSION 3.10)
project(cminiopy CXX)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(miniocpp REQUIRED)

add_library(cminiopy MODULE
    main.cpp
    minio_api.cpp
)

target_include_directories(cminiopy PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cminiopy PRIVATE ${Python3_LIBRARIES})
target_link_libraries(cminiopy PRIVATE miniocpp::miniocpp)

set_target_properties(cminiopy PROPERTIES
    PREFIX ""
    SUFFIX ".pyd"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cminiopy
)
