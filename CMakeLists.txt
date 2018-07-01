#   Copyright 2017 Nikos Efthimiou (nikos.efthimiou AT gmail.com)

#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at

#       http://www.apache.org/licenses/LICENSE-2.0

#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set (CMAKE_CXX_STANDARD 11)

# Set additional project information

PROJECT(SAVVY VERSION 0.5 LANGUAGES CXX)
set(COPYRIGHT "Copyright (c) 2017 Nikos Efthimiou. All rights reserved.")

############################  Options: Build What?  ############################
option(Stir_DOWNLOAD "Downlaod STIR image reconstruction toolkit" OFF)
option(Stir_LOCAL "Use local STIR image reconstruction toolkit" ON)

option(Qwt_DOWNLOAD "Downlaod QWT visualisation" OFF)
option(Qwt_LOCAL "Use local QWT visualisation" ON)

option(BUILD_DOC "Build documentation" ON)

option(ENABLE_TESTING "Enable tests" ON)

############################  Options: Build How?  #############################
include(commons)
option_with_print(BUILD_SHARED_LIBS "Build internally built libraries as shared, not static" OFF)
option_with_default(CMAKE_BUILD_TYPE "Build type (Release or Debug)" Debug)
option_with_default(CMAKE_INSTALL_LIBDIR "Directory to which libraries installed" lib)

if(UNIX)
    # Find the Qt components
    find_package(Qt5 COMPONENTS Core Gui Widgets Test REQUIRED)
endif()

# gcc specific stuff
if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -Wno-deprecated)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=gnu++0x -ffast-math")
endif()

if((${CMAKE_CXX_COMPILER_ID} MATCHES "AppleClang") OR (APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND
        ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# For building the documentation only in Release mode.
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    if (BUILD_DOC)
        find_package(Doxygen)
        if (DOXYGEN_FOUND)
            # set input and output files
            set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

            # request to configure the file
            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
            message("Doxygen build started")

            # note the option ALL which allows to build the docs together with the application
            add_custom_target( doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM )
        else (DOXYGEN_FOUND)
            message("Doxygen need to be installed to generate the doxygen documentation")
        endif()
    endif()
endif()

if (NOT Stir_LOCAL)
    if(NOT Stir_DOWNLOAD)
        message(STATUS "${PROJECT_NAME} depends on STIR. You must either have it installed locally or download them.")
    endif()
else()
    set(Stir_DIR "" CACHE PATH "Root path of STIR.")
    find_package(Stir)
endif()

# Then check if we should download.
if(Stir_DOWNLOAD)
        set(Stir_LOCAL "OFF")

        configure_file(${CMAKE_MODULE_PATH}/stir.txt.in stir-download/CMakeLists.txt)
        execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/stir-download" )
        execute_process(COMMAND "${CMAKE_COMMAND}" --build .
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/stir-download" )
        add_subdirectory("${CMAKE_BINARY_DIR}/stir-src"
                "${CMAKE_BINARY_DIR}/stir-build")

        INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/stir-src/include)
        # Finally the library itself
        find_library(Stir_LIBRARY
          NAMES IO buildblock
          PATHS ${CMAKE_BINARY_DIR}/stir-src/lib ${Stir_PKGCONF_LIBRARY_DIRS}
        )

        set(Stir_FOUND ON)
endif()

if (NOT Qwt_LOCAL)
    if(NOT Qwt_DOWNLOAD)
        message(STATUS "${PROJECT_NAME} depends on QWT. You must either have it installed locally or download them.")
    endif()
else()
    set(Qwt_DIR "" CACHE PATH "Root path of QWT.")
    find_package(Qwt)
endif()

# Then check if we should download.
if(Qwt_DOWNLOAD)
        set(Qwt_LOCAL "OFF")

#        configure_file(${CMAKE_MODULE_PATH}/qwt.txt.in qwt-download/CMakeLists.txt)
        execute_command(COMMAND qmake -pro "${CMAKE_GENERATOR}" .
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/ /qwt-download" )
        execute_process(COMMAND "${CMAKE_COMMAND}" --build .
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qwt-download" )
        add_subdirectory("${CMAKE_BINARY_DIR}/qwt-src"
                "${CMAKE_BINARY_DIR}/qwt-build")

        INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/qwt-src/include)
        # Finally the library itself
        find_library(Qwt_LIBRARY
          NAMES qwt
          PATHS ${CMAKE_BINARY_DIR}/qwt-src/lib ${Qwt_PKGCONF_LIBRARY_DIRS}
        )

        set(Qwt_FOUND ON)
endif()

#
# -- This is required in order to build QT elements
#
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

add_library(savvy_gui)
add_library(savvy_IO)
add_library(savvy_display)

include(src/CMakeLists.txt)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_RESOURCE})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui savvy_gui savvy_IO savvy_display)

if (ENABLE_TESTING)
    enable_testing(true)
    include(src/test/CMakeLists.txt)
else()
    enable_testing(false)
endif()


set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
