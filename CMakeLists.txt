cmake_minimum_required(VERSION 3.5)

project(BlackLogic LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/Lib)
include_directories("Logic")

add_executable(BlackLogic
  main.cpp
  MainBlackLogic/logicgame.cpp
  MainBlackLogic/player.cpp
  MainBlackLogic/action.cpp
  Human/human.cpp
  AI/ai.cpp
  AI/masteraithread.cpp
  AI/aithread.cpp
  AI/event.cpp
  AI/node.cpp

  MainBlackLogic/logicgame.h
  MainBlackLogic/player.h
  MainBlackLogic/action.h
  MainBlackLogic/EnumFun.h
  Human/human.h
  AI/ai.h
  AI/masteraithread.h
  AI/aithread.h
  AI/event.h
  AI/node.hpp
)

link_directories(${LIB_DIR})

target_link_libraries(BlackLogic ${LIB_DIR}/Logic.lib)
install(TARGETS BlackLogic DESTINATION ${CMAKE_SOURCE_DIR}/Lib)
