cmake_minimum_required(VERSION 3.19)

project(CMSC701_Project1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main src/main.cpp)
add_subdirectory(sdsl-lite)
target_link_libraries(main sdsl)
target_include_directories(main PRIVATE ${CMAKE_BINARY_DIR}/sdsl-lite/include/)