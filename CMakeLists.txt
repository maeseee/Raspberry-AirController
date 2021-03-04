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

### COMPILATION OPTIONS MANAGEMENT ###############################################################################################
# See the error with options if they do not switch
# https://stackoverflow.com/questions/22481647/cmake-if-else-with-option#22481880

# Clang is used for more analysis. More strict compiler flags are activated
option(USE_CLANG                   "Build with clang instead of gcc (default=ON)"                                              ON)

option(USE_ADDRESS_SANITIZER       "Enable address sanitizer when available (default=OFF)"                                    OFF)
option(USE_MEMORY_SANITIZER        "Enable memory sanitizer when available (default=OFF)"                                     OFF)
option(USE_THREAD_SANITIZER        "Enable thread sanitizer when available (default=OFF)"                                     OFF)
option(USE_UNDEFINED_SANITIZER     "Enable undefined behavior sanitizer when available (default=OFF)"                         OFF)

option(RUN_CLANG_TIDY              "Enable undefined behavior sanitizer when available (default=ON)"                           ON)
##################################################################################################################################

# Compiler
if(USE_CLANG)
    message(STATUS "Compiler set to clang")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
    
        # set compile flags
    add_compile_options(
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
        -Wdouble-promotion
    )
else()
    message(STATUS "Compiler set to gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

# Sanitizers
if(USE_ADDRESS_SANITIZER)
    message(STATUS "Address sanitizer enabled")
    
    # fno-omit-frame-pointer: meaningful stack traces
    # g: get file names and line numbers in the warning messages
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -g -fsanitize=address")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -g -fsanitize=address")
elseif(USE_MEMORY_SANITIZER) 
    if(NOT USE_CLANG)
        message(SEND_ERROR "To use sanitizers, clang has to bee used")
    endif()
    message(STATUS "Memory sanitizer enabled")
    
    # fno-optimize-sibling-calls: tail call elimination
    # o1: disable inlining
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -fsanitize=memory")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -fsanitize=memory")
elseif(USE_THREAD_SANITIZER)
    if(NOT USE_CLANG)
        message(SEND_ERROR "To use sanitizers, clang has to bee used")
    endif()
    message(STATUS "Thread sanitizer enabled")
    
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=thread")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -fsanitize=thread")
elseif(USE_UNDEFINED_SANITIZER)
    if(NOT USE_CLANG)
        message(SEND_ERROR "To use sanitizers, clang has to bee used")
    endif()
    message(STATUS "Undefined behavior sanitizer enabled")
    
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=undefined")
endif()

# Run clang-tidy and treat all errors as bugs
if(RUN_CLANG_TIDY)
    if(NOT USE_CLANG)
        message(SEND_ERROR "To use clang-tidy, clang has to bee used")
    endif()
    
    set(CMAKE_CXX_CLANG_TIDY clang-tidy -warnings-as-errors=* -checks=-*,bugprone*)
endif()

# set compile independent flags
add_compile_options(
    -std=c++17
    -Wall
    -Werror
    -Wextra
)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O1")

# If enabled, generates a compile_commands.json file containing the exact compiler calls for all translation units
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
