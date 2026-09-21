cmake_minimum_required(VERSION 3.21)

project(CrazyBar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
include(GNUInstallDirs)

# ────────────────────────────────────────
# Box2D
# ────────────────────────────────────────

FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v2.4.1
)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(box2d)

# ────────────────────────────────────────
# Dependencies
# ────────────────────────────────────────

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# ────────────────────────────────────────
# Executable
# ────────────────────────────────────────

add_executable(CrazyBar
        main.cpp
        Box2D_Init.cpp
        LoadRessource.cpp
        RenderWindow.cpp
        Tray.cpp
)

target_include_directories(CrazyBar PRIVATE
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(CrazyBar PRIVATE
        ${SDL2_LIBRARIES}
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
        box2d
)

# ────────────────────────────────────────
# Installation
# ────────────────────────────────────────

install(TARGETS CrazyBar
        RUNTIME DESTINATION .
        BUNDLE DESTINATION .
)

install(DIRECTORY res/
        DESTINATION res
)

# ────────────────────────────────────────
# CPack
# ────────────────────────────────────────

set(CPACK_PACKAGE_NAME "CrazyBar")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CrazyBar game")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR "ZIP")
elseif(UNIX)
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)