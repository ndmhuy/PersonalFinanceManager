cmake_minimum_required(VERSION 3.10)

# 1. Project Name
project(PersonalFinanceManager VERSION 1.0)

# 2. Standard Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 3. Source & Include
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")
find_package(Threads REQUIRED)

# 4. Create Executable
add_executable(PersonalFinanceManager ${SOURCES})
target_link_libraries(PersonalFinanceManager Threads::Threads)

# 5. Auto-Copy Assets
if(NOT EXISTS "${CMAKE_BINARY_DIR}/data")
    message(STATUS "First time setup: Copying default data folder...")
    file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_BINARY_DIR}")
else()
    message(STATUS "Data folder already exists in build. Skipping copy to preserve your changes.")
endif()

message(STATUS "Build setup ready for: ${PROJECT_NAME}")