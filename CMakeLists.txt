cmake_minimum_required(VERSION 3.6)

project(lueftung)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -Werror -Wextra -Wchkp -Wnull-dereference -Waddress -Wsuggest-override -Wduplicated-cond -Wfloat-equal -Wshadow -Wuseless-cast -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

include_directories(src/)

set(dir src/)
# aux_source_directory(${dir} sources)
file (GLOB sources "${dir}/*.cpp")
file (GLOB sourcesSub "${dir}/*/*.cpp")

file (GLOB headers "${dir}/*.hpp")
file (GLOB headersSub "${dir}/*/*.hpp")
# message("My headers: " ${headers})

add_executable(${PROJECT_NAME} ${sources} ${headers} ${sourcesSub} ${headersSub})
target_link_libraries(${PROJECT_NAME} curl)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} boost_system)

add_executable(HTTP_Client tests/TcpClient.cpp)
target_link_libraries(HTTP_Client pthread)
target_link_libraries(HTTP_Client boost_system)
