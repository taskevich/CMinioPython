cmake_minimum_required(VERSION 3.10)
project(cminiopy CXX)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

add_library(cminiopy MODULE
    main.cpp
    minio_api.cpp
)

target_include_directories(cminiopy PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(cminiopy PRIVATE ${Python3_LIBRARIES})

message(STATUS "Python extension suffix: ${Python3_EXTENSION_SUFFIX}")
set_target_properties(cminiopy PROPERTIES
    PREFIX ""
    SUFFIX ".pyd"
)
