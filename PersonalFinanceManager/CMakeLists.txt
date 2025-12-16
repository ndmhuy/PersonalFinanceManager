cmake_minimum_required(VERSION 3.10)
project(PersonalFinanceManager)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Gather all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(PFM ${SOURCES})