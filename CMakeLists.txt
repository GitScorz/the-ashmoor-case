cmake_minimum_required(VERSION 4.0.0)
project(cineris VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Dependencies
set(CINERIS_DEPS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/include")
set(CINERIS_DEPS_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/lib")

# Engine library
file(GLOB_RECURSE CINERIS_ENGINE_SOURCES CONFIGURE_DEPENDS
  engine/src/*.cpp
  engine/src/*.c
)

add_library(cineris_engine STATIC
  ${CINERIS_ENGINE_SOURCES}
  ${CMAKE_SOURCE_DIR}/dependencies/include/glad/glad.c
)

target_include_directories(cineris_engine
  PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/dependencies/include
)

target_link_libraries(cineris_engine
  PUBLIC
    "${CMAKE_SOURCE_DIR}/dependencies/lib/glfw3.lib"
    "${CMAKE_SOURCE_DIR}/dependencies/lib/freetype.lib"
)

target_compile_definitions(cineris_engine
  PUBLIC
    CNR_BASE_PATH="${CMAKE_SOURCE_DIR}/"
)

if(WIN32)
  target_link_libraries(cineris_engine PUBLIC opengl32)
elseif(APPLE)
  find_library(COCOA_LIB Cocoa)
  find_library(IOKIT_LIB IOKit)
  find_library(COREVIDEO_LIB CoreVideo)
  find_library(OPENGL_LIB OpenGL)

  target_link_libraries(cineris_engine PUBLIC
    ${COCOA_LIB}
    ${IOKIT_LIB}
    ${COREVIDEO_LIB}
    ${OPENGL_LIB}
  )
else()
  target_link_libraries(cineris_engine PUBLIC
    GL
    X11
    pthread
    Xrandr
    Xi
    dl
  )
endif()

# Game executable

file(GLOB_RECURSE CINERIS_GAME_SOURCES CONFIGURE_DEPENDS
  game/src/*.cpp
)

add_executable(cineris_game
  ${CINERIS_GAME_SOURCES}
 "game/src/ashmoor_case.cpp")

target_link_libraries(cineris_game
  PRIVATE
    cineris_engine
)

target_include_directories(cineris_game
  PRIVATE
    ${CMAKE_SOURCE_DIR}/game/src
)

# Copy runtime assets

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

add_custom_command(TARGET cineris_game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/shaders"
          "$<TARGET_FILE_DIR:cineris_game>/shaders"
  COMMENT "Copying shaders to output folder."
)

add_custom_command(TARGET cineris_game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/resources"
          "$<TARGET_FILE_DIR:cineris_game>/resources"
  COMMENT "Copying resources to output folder."
)

if(WIN32)
  add_custom_command(TARGET cineris_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/dependencies/lib/freetype.dll"
            "$<TARGET_FILE_DIR:cineris_game>"
    COMMENT "Copying FreeType DLL to output folder."
  )

  add_custom_command(TARGET cineris_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/dependencies/lib/glfw3.dll"
            "$<TARGET_FILE_DIR:cineris_game>"
    COMMENT "Copying GLFW DLL to output folder."
  )
endif()
