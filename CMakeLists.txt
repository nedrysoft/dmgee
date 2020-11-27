#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of dmgee
#
# Created by Adrian Carpenter on 29/10/2020.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

project(dmgee)

# set up some variables to describe the application

set(APPLICATION_SHORT_NAME "dmgee")
set(APPLICATION_LONG_NAME "dmgee")

if((APPLE) AND (CMAKE_OSX_ARCHITECTURES MATCHES "arm64"))
    set(APPLICATION_PLATFORM_ARCH "arm64")
else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(APPLICATION_PLATFORM_ARCH "x86_64")
    else()
        set(APPLICATION_PLATFORM_ARCH "x86_32")
    endif()
endif()

add_subdirectory(thirdparty/yaml-cpp)

set(APPLICATON_SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}")

set(APPLICATION_BIN_OUTPUT "${CMAKE_CURRENT_LIST_DIR}/bin/${APPLICATION_PLATFORM_ARCH}/${CMAKE_BUILD_TYPE}")

set(FRAMEWORK_OUTPUT "${CMAKE_CURRENT_LIST_DIR}/bin/${APPLICATION_PLATFORM_ARCH}/${CMAKE_BUILD_TYPE}/${APPLICATION_SHORT_NAME}.app/Contents/Frameworks")

set(BUNDLE_DIR "${CMAKE_CURRENT_LIST_DIR}/bin/${APPLICATION_PLATFORM_ARCH}/${CMAKE_BUILD_TYPE}/${APPLICATION_SHORT_NAME}.app/Contents")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# disable Qt keyboards as they clash with Python

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

# define required for python

ADD_DEFINITIONS(-DPY_SSIZE_T_CLEAN)

# disable TOML exceptions

ADD_DEFINITIONS(-DTOML_EXCEPTIONS=0)

# list of packages we need for the application

find_package(Python3 COMPONENTS Development REQUIRED)
find_package(OpenCV REQUIRED)
find_package(DevIL REQUIRED)
find_package(Git QUIET)

# the source files

list(APPEND app_SOURCES
    src/AboutDialog.cpp
    src/AboutDialog.h
    src/AboutDialog.ui
    src/AnsiEscape.cpp
    src/AnsiEscape.h
    src/Builder.cpp
    src/Builder.h
    src/BulletWidget.cpp
    src/BulletWidget.h
    src/FlatTabBar.cpp
    src/FlatTabBar.h
    src/FlatTabWidget.cpp
    src/FlatTabWidget.h
    src/GeneralSettingsPage.cpp
    src/GeneralSettingsPage.h
    src/GeneralSettingsPage.ui
    src/GridGraphicsScene.cpp
    src/GridGraphicsScene.h
    src/HTermApi.cpp
    src/HTermApi.h
    src/HTermWidget.cpp
    src/HTermWidget.h
    src/Helper.cpp
    src/Helper.h
    src/ILicense.h
    src/ISettingsPage.h
    src/Image.cpp
    src/Image.h
    src/LicenceTemplatesSettingsPage.cpp
    src/LicenceTemplatesSettingsPage.h
    src/LicenceTemplatesSettingsPage.ui
    src/MacHelper.h
    src/MacHelper.mm
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
    src/PreviewWidget.cpp
    src/PreviewWidget.h
    src/Python.cpp
    src/Python.h
    src/SettingsDialog.cpp
    src/SettingsDialog.cpp
    src/SettingsDialog.h
    src/SettingsDialog.h
    src/SnappedGraphicsPixmapItem.cpp
    src/SnappedGraphicsPixmapItem.h
    src/SplashScreen.cpp
    src/SplashScreen.h
    src/ThemeSupport.h
    src/ThemeSupport.mm
    src/ThemedOutlineView.cpp
    src/ThemedOutlineView.h
    src/ThemedOutlineViewButtonBox.cpp
    src/ThemedOutlineViewButtonBox.h
    src/TransparentWidget.cpp
    src/TransparentWidget.h
    src/hterm.qrc
    src/main.cpp

    src/app.qrc
    icons/dmgee.icns

    licences/licences.qrc
)

list(APPEND all_SOURCES ${app_SOURCES})

# Create an application executable (and ensure it's an application bundle under macOS)

set(MACOSX_BUNDLE_ICON_FILE dmgee.icns)
set(dmgee_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icons/dmgee.icns)
set_source_files_properties(${dmgee_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(${APPLICATION_SHORT_NAME} MACOSX_BUNDLE ${all_SOURCES})

option(BUILD_OPTION_COPY_PYTHON "Copy Python frameworks to bundle." OFF)

if(BUILD_OPTION_COPY_PYTHON)
    # copy our python script and it's dependant modules

    add_custom_command(TARGET ${APPLICATION_SHORT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/python
        ${BUNDLE_DIR}/Resources/python
        DEPENDS
        "${PROJECT_SOURCE_DIR}/python"
    )

    # create the path to the python framework so we can copy it to the bundle

    string(REPLACE "/" ";" Python3_Path ${Python3_LIBRARY})

    set(Python3_Framework "")

    foreach(currentPart IN LISTS Python3_Path)
        if (currentPart STREQUAL "")
            continue()
        endif()

        string(APPEND Python3_Framework "/" ${currentPart})

        if (currentPart STREQUAL "Python.framework")
            break()
        endif()
    endforeach()

    # copy the python framework to our bundle

    add_custom_command(TARGET ${APPLICATION_SHORT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${Python3_Framework}
        ${FRAMEWORK_OUTPUT}/Python.framework
    )
endif()

# create lists for includes

target_include_directories(${APPLICATION_SHORT_NAME} SYSTEM PUBLIC
    "${IL_INCLUDE_DIR}/../"
)

target_include_directories(${APPLICATION_SHORT_NAME} PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    thirdparty
    thirdparty/tomlplusplus/include
    thirdparty/fmt/include
    thirdparty/CLI11/include
    libs/
    src
    dmgee_BINARY_DIR/${PROJECT_NAME}_autogen
    thirdparty/yaml-cpp/include
)

# find objective c sources and set options

foreach(currentSourceFile IN LISTS application_SOURCES)
    get_filename_component(fileExtension ${currentSourceFile} EXT)

    string(TOLOWER ${fileExtension} lowerCaseExtension)

    if("${lowerCaseExtension}" STREQUAL ".mm")
        set_source_files_properties(${currentSourceFile} PROPERTIES COMPILE_OPTIONS "-fobjc-weak")
    endif()
endforeach(currentSourceFile)

# Generate a qrc containing the license files

set(LicenceFiles "")

string(APPEND LicenceFiles "<RCC>\r\n")
string(APPEND LicenceFiles "  <qresource prefix=\"/\">\r\n")

file(GLOB files "${APPLICATON_SOURCE_ROOT}/licences/choosealicence.com/_licenses/*.txt")

foreach(file ${files})
    get_filename_component(licenseFilename ${file} NAME)

    string(APPEND LicenceFiles "    <file>./choosealicence.com/_licenses/${licenseFilename}</file>\r\n")
endforeach()

file(GLOB files "${APPLICATON_SOURCE_ROOT}/licences/spdx/json/details/*.json")

foreach(file ${files})
    get_filename_component(licenseFilename ${file} NAME)

    string(APPEND LicenceFiles "    <file>./spdx/json/details/${licenseFilename}</file>\r\n")
endforeach()

# add rules yml

string(APPEND LicenceFiles "    <file>./choosealicence.com/_data/rules.yml</file>\r\n")

string(APPEND LicenceFiles "  </qresource>\r\n")

string(APPEND LicenceFiles "</RCC>\r\n")

file(GENERATE OUTPUT "${APPLICATON_SOURCE_ROOT}/licences/licences.qrc" CONTENT "${LicenceFiles}" TARGET ${APPLICATION_SHORT_NAME})

set_source_files_properties(${APPLICATON_SOURCE_ROOT}/licences/licences.qrc PROPERTIES GENERATED TRUE)

# add the libraries we need to link to

target_link_libraries(${APPLICATION_SHORT_NAME} "-framework Cocoa -framework IOKit -framework CoreVideo -framework QuartzCore")
target_link_libraries(${APPLICATION_SHORT_NAME} "-L${Python3_LIBRARY_DIRS} ${Python3_LIBRARIES}")
target_link_libraries(${APPLICATION_SHORT_NAME} ${OpenCV_LIBS} )
target_link_libraries(${APPLICATION_SHORT_NAME} ${IL_LIBRARIES} ${ILU_LIBRARIES})
target_link_libraries(${APPLICATION_SHORT_NAME} -L/Users/adriancarpenter/Documents/Development/dmgee/cmake-build-debug/thirdparty/yaml-cpp)
target_link_libraries(${APPLICATION_SHORT_NAME} -lyaml-cppd)

# List of Qt libraries used by the application

set(APPLICATION_QT_LIBRARIES
    Core
    Gui
    Widgets
    QuickWidgets
    WebEngineWidgets
    MacExtras
)

# Check that the Qt libraries are available

find_package(Qt5 COMPONENTS ${APPLICATION_QT_LIBRARIES} REQUIRED)
find_package(Qt5 COMPONENTS LinguistTools)

# Set the target executable name

set_target_properties(${APPLICATION_SHORT_NAME} PROPERTIES OUTPUT_NAME "${APPLICATION_LONG_NAME}")

# generate version information

execute_process(COMMAND git --git-dir=${APPLICATON_SOURCE_ROOT}/.git log -1 --format=%cd --date=format:%Y OUTPUT_VARIABLE APPLICATION_GIT_YEAR)
execute_process(COMMAND git --git-dir=${APPLICATON_SOURCE_ROOT}/.git log -1 --format=%cd --date=format:%m OUTPUT_VARIABLE APPLICATION_GIT_MONTH)
execute_process(COMMAND git --git-dir=${APPLICATON_SOURCE_ROOT}/.git log -1 --format=%cd --date=format:%d OUTPUT_VARIABLE APPLICATION_GIT_DAY)
execute_process(COMMAND git --git-dir=${APPLICATON_SOURCE_ROOT}/.git log -1 --format=%h OUTPUT_VARIABLE APPLICATION_GIT_HASH)
execute_process(COMMAND git --git-dir=${APPLICATON_SOURCE_ROOT}/.git branch --show-current OUTPUT_VARIABLE APPLICATION_GIT_BRANCH)
execute_process(COMMAND git --git-dir=${APPLICATON_SOURCE_ROOT}/.git diff --quiet HEAD RESULT_VARIABLE APPLICATION_GIT_UNCOMMITED)

if (APPLICATION_GIT_UNCOMMITED EQUAL 1)
    set(APPLICATION_GIT_UNCOMMITED "+")
else()
    set(APPLICATION_GIT_UNCOMMITED "")
endif()

if ("${APPLICATION_GIT_YEAR}" STREQUAL "")
    set(APPLICATION_GIT_YEAR 0)
    set(APPLICATION_GIT_MONTH 0)
    set(APPLICATION_GIT_DAY 0)
    set(APPLICATION_GIT_BRANCH none)
    set(APPLICATION_GIT_HASH 0)
else()
    string(STRIP "${APPLICATION_GIT_YEAR}" APPLICATION_GIT_YEAR)
    string(STRIP "${APPLICATION_GIT_MONTH}" APPLICATION_GIT_MONTH)
    string(STRIP "${APPLICATION_GIT_DAY}" APPLICATION_GIT_DAY)
    string(STRIP "${APPLICATION_GIT_BRANCH}" APPLICATION_GIT_BRANCH)
    string(STRIP "${APPLICATION_GIT_HASH}" APPLICATION_GIT_HASH)
endif()

# Add preprocessor definitions

add_definitions("-DAPPLICATION_GIT_YEAR=\"${APPLICATION_GIT_YEAR}\"")
add_definitions("-DAPPLICATION_GIT_MONTH=\"${APPLICATION_GIT_MONTH}\"")
add_definitions("-DAPPLICATION_GIT_DAY=\"${APPLICATION_GIT_DAY}\"")
add_definitions("-DAPPLICATION_GIT_BRANCH=\"${APPLICATION_GIT_BRANCH}\"")
add_definitions("-DAPPLICATION_GIT_HASH=\"${APPLICATION_GIT_HASH}\"")
add_definitions("-DAPPLICATION_LONG_NAME=\"${APPLICATION_LONG_NAME}\"")
add_definitions("-DAPPLICATION_SHORT_NAME=\"${APPLICATION_SHORT_NAME}\"")

# install git submodules as required

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")

        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# add frameworks to rpath

set_target_properties(${APPLICATION_SHORT_NAME} PROPERTIES XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/Frameworks")

# add our Frameworks folder to the rpath

add_custom_command(TARGET ${APPLICATION_SHORT_NAME} POST_BUILD
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path/../Frameworks/"
        $<TARGET_FILE:dmgee>
)

set(APPLICATION_QT_LINK_LIBRARIES "")

foreach(arg IN ITEMS ${APPLICATION_QT_LIBRARIES})
    list(APPEND APPLICATION_QT_LINK_LIBRARIES "Qt5::${arg}")
endforeach()

# Set the target executable name

set_target_properties(${APPLICATION_SHORT_NAME} PROPERTIES OUTPUT_NAME "${APPLICATION_LONG_NAME}")

# Set the folder where the binary should be placed

set_target_properties(${APPLICATION_SHORT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${APPLICATION_BIN_OUTPUT})

# Ensure the icon is copied to the bundle and set up the Info.plist file from the template

set_source_files_properties("assets/${APPLICATION_SHORT_NAME}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set_target_properties(${APPLICATION_SHORT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${APPLICATON_SOURCE_ROOT}/Info.plist.in)

set(APPLICATION_BUNDLE_NAME "${CMAKE_PROJECT_NAME}")
set(APPLICATION_BUNDLE_ICONS "${APPLICATION_SHORT_NAME}.icns")
set(APPLICATION_BUNDLE_IDENTIFIER "com.nedrysoft.${APPLICATION_SHORT_NAME}")
set(APPLICATION_BUNDLE_SCHEME "${APPLICATION_SHORT_NAME}")

# cocoa frameworks

list(APPEND APPLICATION_MAC_LINK_LIBRARIES "-framework AppKit")

# Set the libraries be linked in

target_link_libraries(${APPLICATION_SHORT_NAME} ${APPLICATION_QT_LINK_LIBRARIES} ${APPLICATION_MAC_LINK_LIBRARIES})
target_link_libraries(${APPLICATION_SHORT_NAME} Ribbon)

add_subdirectory("libs/Ribbon")

# translations

qt5_create_translation(QM_FILES ${app_SOURCES}
    ${APPLICATON_SOURCE_ROOT}/translations/dmgee_en.ts
    ${APPLICATON_SOURCE_ROOT}/translations/dmgee_fr.ts
)
