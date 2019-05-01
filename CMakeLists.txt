#
#   clove
#
#   Copyright (C) 2016-2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
#

cmake_minimum_required(VERSION 2.8)

project(clove)

option(USE_PHYSFS "Build Clove using Physfs for filesystem?" ON)
option(USE_LUA "Build CLove using Lua as scripting language" OFF)
option(USE_FH "Build CLove using FH as scripting language" ON)

option(STATIC_LIB "Builds CLove as a static library" ON)
option(DYNAMIC_LIB "Builds CLove as a shared library" OFF)

if (USE_PHYSFS)
    ADD_DEFINITIONS(-DUSE_PHYSFS=ON)
    message(STATUS, "Using Physfs")
endif(USE_PHYSFS)

if (USE_LUA)
    #ADD_DEFINITIONS(-DUSE_LUA=ON)
    #message(STATUS, "Using Lua")
endif(USE_LUA)

if (USE_FH)
    ADD_DEFINITIONS(-DUSE_FH=ON)
    message(STATUS, "Using FH")
endif(USE_FH)

IF(WIN32)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MOE_BUILD_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Z7 /W4 /Od /MDd /D_DEBUG /D__Debug__")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /W1 /Ox /Ot /MD /DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /GR /FC /DWIN32 /D_MBCS")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7 /W4 /Od /MDd /D_DEBUG /D__Debug__")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W1 /Ox /Ot /MD /DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS "")
FIND_PACKAGE(OpenGL REQUIRED)
SET(OPENAL_LIBRARY "OpenAL32")
set(MTCL_BUILD_FLAGS "")
set(OS_SPECIFIC_LIBRARIES)
ENDIF()

IF(UNIX AND NOT APPLE)
FIND_PACKAGE(OpenGL REQUIRED)
SET(OPENAL_LIBRARY "openal")
set(CMAKE_C_FLAGS " -std=gnu99 -Wvla -Wall -static-libgcc ${CMAKE_C_FLAGS} -fPIC -DLINUX -ldl")
set(CMAKE_C_FLAGS_DEBUG "  -std=c99 ${CMAKE_C_FLAGS_DEBUG} -D__Debug__")
set(CMAKE_C_FLAGS_RELEASE " -std=c99 ${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MTCL_BUILD_FLAGS} -fPIC -DLINUX -ldl")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__Debug__")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_SHARED_LINKER_FLAGS "-ldl -Wl,-rpath=$ORIGIN,--as-needed,--allow-multiple-definition,--build-id")
set(OS_SPECIFIC_LIBRARIES pthread Xxf86vm X11 Xrandr Xi)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "install")
ENDIF()

if (APPLE)
    LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    IF("${isSystemDir}" STREQUAL "-1")
    ENDIF("${isSystemDir}" STREQUAL "-1")
    FIND_PACKAGE(OpenGL REQUIRED)
    find_library(COCOA_LIB Cocoa)
    find_library(CORE_SERVICES_LIB CoreServices)
    find_library(FOUNDATION_LIB Foundation)
    find_library(IOKIT_LIB IOKit)
    FIND_LIBRARY(OPENAL_LIBRARY OpenAL)
    set(MTCL_BUILD_FLAGS "-mmacosx-version-min=10.6")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MTCL_BUILD_FLAGS} -Wall -Wvla -O3 -DOSX")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__Debug__")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MTCL_BUILD_FLAGS} -O3 -DOSX")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__Debug__")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl")
    set(OS_SPECIFIC_LIBRARIES ${COCOA_LIB} ${CORE_SERVICES_LIB} ${FOUNDATION_LIB} ${IOKIT_LIB})
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_INSTALL_RPATH "install")
endif()

if (ANDROID)
    #FIND_PACKAGE(OpenGLES REQUIRED)
    #FIND_PACKAGE(OpenGLES2 REQUIRED)

    set (ARM_FLAGS "-march=armv6 -marm -mfloat-abi=softfp -mfpu=vfp")
    set ( ANDROID_FORCE_ARM_BUILD ON )
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -DNDEBUG -DANDROID_NDK -DDISABLE_IMPORTGL" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -DNDEBUG -DANDROID_NDK -DDISABLE_IMPORTGL" )
    ADD_DEFINITIONS("-Dva_list=__builtin_va_list")
    ADD_DEFINITIONS("-Dva_start=__builtin_va_start")
    ADD_DEFINITIONS("-Dva_end=__builtin_va_end")
    ADD_DEFINITIONS("-Dva_arg=__builtin_va_arg")
    ADD_DEFINITIONS("-DHAVE_MALLOC")
    ADD_DEFINITIONS("-DANDROID")
    ADD_DEFINITIONS("-DUSE_GLES")
    ADD_DEFINITIONS("-D__ANDROID__")

    SET(LIBTYPE STATIC)
endif()

FILE(
    GLOB SOURCE_FILES
    src/*.c
    src/*.h
    src/audio/*.h
    src/audio/*.c
    src/filesystem/*.h
    src/filesystem/*.c
    src/graphics/*.h
    src/graphics/*.c
    src/image/*.h
    src/image/*.c
    src/tools/*.c
    src/tools/*.h
    src/math/*.h
    src/math/*.c
    src/timer/*.h
    src/timer/*.c
    src/net/*.c
    src/net/*.h
    src/physics/*.c
    src/physics/*.h
    src/include/*.h
    src/include/*.c

    #Add custom native plugins
    native/*.c*
    native/*.h

    #3rdparty "small" libs
    src/3rdparty/microtar/*.c
    src/3rdparty/microtar/*.h
    src/3rdparty/noise/*.c
    src/3rdparty/noise/*.h
    src/3rdparty/slre/*.c
    src/3rdparty/slre/*.h
    src/3rdparty/CMath/*.h
    src/3rdparty/CMath/*.c
    )

if (USE_LUA)
    FILE(
        GLOB SOURCE_LUA_FILES
        src/luaapi/*.h
        src/luaapi/*.c
        )
endif()

if (USE_FH)
    FILE(
        GLOB SOURCE_FH_FILES
        src/fhapi/*.h
        src/fhapi/*.c
        src/3rdparty/FH/src/*.c
        src/3rdparty/FH/src/*.h
        src/3rdparty/FH/src/map/*.c
        src/3rdparty/FH/src/map/*.h
        src/3rdparty/FH/src/vec/*.c
        src/3rdparty/FH/src/vec/*.h
        )
endif()

set(clove_package "clove")
set(clove_creator "Muresan Vlad")
set(clove_copyright "Copyright (c) 2015-2019, ${clove_creator}.")
message(${clove_copyright})

if (STATIC_LIB)
    add_library(love STATIC ${SOURCE_FILES})
endif(STATIC_LIB)

if (SHARED_LIB)
    add_library(love_shared SHARED ${SOURCE_FILES})
endif(SHARED_LIB)

if (NOT ANDROID)
    add_executable(
        clove
        ${SOURCE_FILES}
        ${SOURCE_FH_FILES}
    #    $(SOURCE_LUA_FILES)
        )
endif()

if (ANDROID)

    set(INCLUDE_CLOVEANDROID
        "src/"
        "3rdparty/"
        )

    include_directories( ${INCLUDE_CLOVEANDROID}	)
    add_library( clove SHARED ${SOURCE_FILES} )

endif()

#chipmunk2d
#add_subdirectory(src/3rdparty/Chipmunk2D)
#set(CHIPMUNK_INCLUDE_DIR src/3rdparty/Chipmunk2D/include)
#set(CHIPMUNK_LIBRARIES chipmunk)

#zlib
if (USE_PHYSFS)
    set(ZLIB_FOUND TRUE)
    add_subdirectory(src/3rdparty/zlib)
    set(ZLIB_INCLUDE_DIR src/3rdparty/zlib)
    set(ZLIB_LIBRARIES zlibstatic)
endif(USE_PHYSFS)

#freetype
add_subdirectory(src/3rdparty/freetype)
include_directories(src/3rdparty/freetype/include)
set(FREETYPE_LIBRARIES freetype)

#openal
add_subdirectory(src/3rdparty/openal)
set(OPENAL_INCLUDE_DIR src/3rdparty/openal/include/)

#lua
if (USE_LUA)
    add_subdirectory(src/3rdparty/lua)
    set(LUA_INCLUDE_DIR src/3rdparty/lua/)
    set(LUA_LIBRARIES lua)
endif(USE_LUA)

#glew
add_subdirectory(src/3rdparty/glew/)
set(GLEW_INCLUDE_DIR src/3rdparty/glew/include/GL/)
set(GLEW_LIBRARIES libglew_static)

#sdl2
add_subdirectory(src/3rdparty/SDL2/)
set(SDL_INCLUDE_DIR src/3rdparty/SDL2/include/)
set(SDL_LIBRARIES SDL2-static SDL2main)

#physfs
if (USE_PHYSFS)
    add_subdirectory(src/3rdparty/physfs)
    set(PHYSFS_INCLUDE_DIR src/3rdparty/physfs)
    set(PHYSFS_LIBRARIES physfs-static)
endif(USE_PHYSFS)

if(WIN32)
    set(OPENAL_LIBRARIES "OpenAL32")
else()
    set(OPENAL_LIBRARIES openal)
endif()


add_definitions(-DFT2_BUILD_LIBRARY)
add_definitions(-DSDL2)


if(WIN32)

    target_link_libraries(clove ws2_32.lib
        ${SDL_LIBRARIES} ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES} ${OPENAL_LIBRARIES}
        ${FREETYPE_LIBRARIES})

    if (USE_PHYSFS)
        target_link_libraries(clove ${PHYSFS_LIBRARIES} ${ZLIB_LIBRARIES})
    endif(USE_PHYSFS)

    if (USE_LUA)
        target_link_libraries(clove ${LUA_LIBRARIES})
    endif(USE_LUA)

else()
    target_link_libraries(clove
        ${SDL_LIBRARIES} ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${FREETYPE_LIBRARIES} ${OPENAL_LIBRARIES})
    #${CHIPMUNK_LIBRARIES})

    if (USE_PHYSFS)
        target_link_libraries(clove ${PHYSFS_LIBRARIES} ${ZLIB_LIBRARIES})
    endif(USE_PHYSFS)

    if (USE_LUA)
        target_link_libraries(clove ${LUA_LIBRARIES})
    endif(USE_LUA)

endif(WIN32)

if(ANDROID)
    target_link_libraries(clove
        ${SDL_LIBRARIES} ${OPENGL_LIBRARIES}
        ${OPENAL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LUA_LIBRARIES} )
endif()
