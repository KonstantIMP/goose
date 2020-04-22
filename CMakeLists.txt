#Minimal CMake Version
cmake_minimum_required(VERSION 3.6)

#Project name
project(goose LANGUAGES CXX)

#C++ prefernces
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#SFML libraries
set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

#Main game code
set(GAME_SOURCE src/main.cpp)

#Error message code
set(ERR_MSG src/err_msg.cpp)

#Options window code
set(SETTING src/option.cpp)

#Main game library
set(GAME_LIB src/game.cpp
             include/game.hpp)

#Config file library
set(CONFIG_LIB src/config.cpp
               include/config.hpp)

#Input manager library
set(INPUT_LIB src/input_manager.cpp
              include/input_manager.hpp)

#SFML search
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

message("   *** Lua include: ${LUA_INCLUDE_DIR}")
message("   *** Lua lib: ${LUA_LIBRARIES}")

#GTK+ search
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})
add_compile_options(${GTK3_CFLAGS_OTHER})
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GTK3_FLAGS} -Wl,--export-dynamic")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GTK3_FLAGS} -Wl,--export-dynamic")

#Main game library creating
add_library(game STATIC ${GAME_LIB})
target_link_libraries(game ${SFML_LIBRARIES})

#Input manager library creating
add_library(input_mgr SHARED ${INPUT_LIB})
target_link_libraries(input_mgr ${SFML_LIBRARIES})

#Config library creating
add_library(config SHARED ${CONFIG_LIB})

#Programm files creting
add_executable(goose ${GAME_SOURCE})
add_executable(error_msg ${ERR_MSG})
add_executable(options ${SETTING})

#Linking libraryes to binary files
target_link_libraries(error_msg ${GTK3_LIBRARIES})
target_link_libraries(options ${GTK3_LIBRARIES} config)
target_link_libraries(goose game input_mgr config)


############################# Tests ##################################


######################################################################
