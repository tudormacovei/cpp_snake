cmake_minimum_required(VERSION 3.13)  # CMake version check
project(cpp_snake_game)               # Create project "cpp_snake_game
set(CMAKE_CXX_STANDARD 17)

# Declare variable with compile flags, pedantic-errors makes sure that we adhere to the C++ standard
# And that no compiler extensions will be allowed
set(GCC_COVERAGE_COMPILE_FLAGS "-pedantic-errors -Weffc++ -Wall -Wextra -Wconversion -Wsign-conversion")

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES main.cpp)

# Set compile flags
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(cpp_snake_game ${SOURCE_FILES})