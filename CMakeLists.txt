cmake_minimum_required(VERSION 3.6)

project(goose LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

set(GAME_SOURCE src/main.cpp)

set(ERR_MSG src/err_msg.cpp)

set(OPTION src/option.cpp)

set(GAME_LIB src/game.cpp
             include/game.hpp)

set(INPUT_LIB src/input_manager.cpp
              include/input_manager.hpp)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

add_library(game STATIC ${GAME_LIB})

target_link_libraries(game ${SFML_LIBRARIES})

add_library(input_mgr SHARED ${INPUT_LIB})

target_link_libraries(input_mgr ${SFML_LIBRARIES})

add_executable(goose ${GAME_SOURCE})

add_executable(error_msg ${ERR_MSG})

target_link_libraries(error_msg ${GTK3_LIBRARIES})

target_link_libraries(goose game input_mgr)
