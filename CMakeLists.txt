cmake_minimum_required(VERSION 3.5)

project(BlackLogic LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/../../Lib)
include_directories("Logic")

add_executable(BlackLogic
  main.cpp
  MainBlackLogic/logicgame.cpp
  MainBlackLogic/logicgame.h
)

target_link_libraries(BlackLogic ${LIB_DIR}/libLogic.a)
