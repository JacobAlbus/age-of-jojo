cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(age-of-jojo)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wpedantic -Werror)
endif()

include(FetchContent)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.x
)

FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_library(catch2 INTERFACE)
    target_include_directories(catch2 INTERFACE ${catch2_SOURCE_DIR}/single_include)
endif()

FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.7.3/include.zip
)
# Adds nlohmann_json library. This is a header only library
FetchContent_GetProperties(nlohmann_json)
if(NOT nlohmann_json_POPULATED)
    FetchContent_Populate(nlohmann_json)
    add_library(nlohmann_json INTERFACE)
    target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR}/single_include)
endif()

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

#list(APPEND ENGINE_SOURCE_FILES src/engine/board.cc src/engine/tile.cc src/engine/main_menu_engine.cpp
#                                src/engine/board_game_engine.cc src/engine/pause_menu_engine.cpp src/engine/character.cc
#                                src/engine/level_select_menu_engine.cpp)

list(APPEND GUI_SOURCE_FILES src/gui/jjba_strategy_app.cc)

#list(APPEND TEST_FILES tests/board_tests.cc tests/character_tests.cc tests/game_engine_tests.cc)

ci_make_app(
        APP_NAME        jjba-visualizer
        CINDER_PATH     ${CINDER_PATH}
        SOURCES jjba_strategy_main.cc ${GUI_SOURCE_FILES} ${ENGINE_SOURCE_FILES}
        INCLUDES        include
        LIBRARIES       nlohmann_json
)

#ci_make_app(
#        APP_NAME        jjba-tests
#        CINDER_PATH     ${CINDER_PATH}
#        SOURCES tests/test_main.cc ${ENGINE_SOURCE_FILES} ${GUI_SOURCE_FILES} ${TEST_FILES}
#        INCLUDES        include
#        LIBRARIES       catch2 nlohmann_json
#)

if(MSVC)
    set_property(TARGET APPEND_STRING PROPERTY LINK_FLAGS " /SUBSYSTEM:CONSOLE")
endif()