cmake_minimum_required(VERSION 3.6)

project(lueftung)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -Werror -Wextra -Wno-conversion -Wnull-dereference -Waddress -Wfloat-equal -Wfloat-conversion -Wshadow -Wpedantic -Wunused-value -Wold-style-cast -Winit-self")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++11 -O0")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/Mosquitto)

set(dir src/)
# aux_source_directory(${dir} sources)
file (GLOB sources "${dir}/*.cpp")
file (GLOB sourcesSub "${dir}/*/*.cpp")

file (GLOB headers "${dir}/*.hpp")
file (GLOB headersSub "${dir}/*/*.hpp")

add_executable(${PROJECT_NAME} ${sources} ${headers} ${sourcesSub} ${headersSub})
target_link_libraries(${PROJECT_NAME} curl pthread boost_system)

add_executable(HTTP_Client tests/TcpClient.cpp)
target_link_libraries(HTTP_Client pthread boost_system)
