cmake_minimum_required(VERSION 3.12)

project(CSnake LANGUAGES CXX)

find_package(Torch REQUIRED)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS})

include_directories(include)

add_executable(CSnake src/main.cpp src/game.cpp src/apple.cpp src/snake.cpp src/model.cpp src/agent.cpp)

target_link_libraries(CSnake ${SDL2_LIBRARIES} ${TORCH_LIBRARIES})
target_link_libraries(CSnake SDL2_ttf)
