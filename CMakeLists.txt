cmake_minimum_required(VERSION 2.8)

project(lueftung)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(src/)

aux_source_directory(src/ lueftung_SRC)

add_executable(${PROJECT_NAME} ${lueftung_SRC})
target_link_libraries(${PROJECT_NAME} curl)
target_link_libraries(${PROJECT_NAME} pthread)

