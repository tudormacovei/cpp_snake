cmake_minimum_required(VERSION 3.13)  # CMake version check
project(cpp_snake_game)               # Create project "cpp_snake_game
set(CMAKE_CXX_STANDARD 17)

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES main.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(cpp_snake_game ${SOURCE_FILES})