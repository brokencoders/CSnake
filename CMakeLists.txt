# Every file start with this, it's set the minimum required
# version of cmake to work with
cmake_minimum_required(VERSION 3.12)

# This set your project name and the compiling lenguage
project(CSnake LANGUAGES CXX)

# Find SDL and includes header files
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS})


# Add include directory to those the compiler uses to search for include files 
include_directories(include)

# Add an executable to the project using the specified source files
add_executable(CSnake src/main.cpp src/game.cpp src/entities.cpp)

# Link SDL to the executable
target_link_libraries(CSnake ${SDL2_LIBRARIES})
target_link_libraries(CSnake SDL2_ttf)
