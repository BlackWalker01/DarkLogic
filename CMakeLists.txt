cmake_minimum_required(VERSION 3.5)

project(LogicGame LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/Lib)
include_directories("DarkLogic")

add_executable(LogicGame
  main.cpp
  MainDarkLogic/logicgame.cpp
  MainDarkLogic/player.cpp
  MainDarkLogic/action.cpp
  Human/human.cpp
  AI/ai.cpp
  AI/masteraithread.cpp
  AI/aithread.cpp
  AI/event.cpp
  AI/node.cpp

  MainDarkLogic/logicgame.h
  MainDarkLogic/player.h
  MainDarkLogic/action.h
  MainDarkLogic/EnumFun.h
  Human/human.h
  AI/ai.h
  AI/masteraithread.h
  AI/aithread.h
  AI/event.h
  AI/node.hpp
)

target_link_directories(LogicGame PUBLIC ${LIB_DIR})

add_compile_definitions(_SILENCE_CLANG_CONCEPTS_MESSAGE)

if(UNIX)
    target_link_libraries(LogicGame DarkLogic pthread stdc++ m)
else()
    target_link_libraries(LogicGame DarkLogic)
endif()

install(TARGETS LogicGame DESTINATION ${CMAKE_SOURCE_DIR}/Lib)
