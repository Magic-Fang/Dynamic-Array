cmake_minimum_required(VERSION 2.8)
project(darray)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

include_directories(
        src
)

set(SOURCE_FILES
        src/array.h
        src/array.cc
        src/simple_string.h
        src/simple_string.cc
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES} main.cc)