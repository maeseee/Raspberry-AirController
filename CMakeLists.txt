cmake_minimum_required(VERSION 3.6)

# Prerequirements:
# sudo apt-get install libboost-all-dev clang cmake libcurl4-nss-dev

# Compile a release version
# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make

# Compile a debug version
# mkdir buildDebug
# cd buildDebug
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# make

project(lueftung)

# choose compiler
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# set compile flags
add_compile_options(
  -std=c++17
  -Wall
  -Werror
  -Wextra
  -Waddress
  -Wnull-dereference
  -Waddress
  -Wfloat-equal
  -Wfloat-conversion
  -Wshadow
  -Wpedantic
  -Wunused-value
  -Wold-style-cast
  -Woverloaded-virtual
  -Winit-self
  #-Wzero-as-null-pointer-constant
  )

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add clang address sanitizer to debug version
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

include_directories(src/)

set(dir src/)
file (GLOB sources "${dir}/*.cpp")
file (GLOB sourcesSub "${dir}/*/*.cpp")

file (GLOB headers "${dir}/*.hpp")
file (GLOB headersSub "${dir}/*/*.hpp")

add_executable(${PROJECT_NAME} ${sources} ${headers} ${sourcesSub} ${headersSub})
target_link_libraries(${PROJECT_NAME} curl pthread boost_system)

add_executable(HTTP_Client tests/TcpClient.cpp)
target_link_libraries(HTTP_Client pthread boost_system)
