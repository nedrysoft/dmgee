#
# Copyright (C) 2020 Adrian Carpenter
#
# https://github.com/fizzyade
#
# Created by Adrian Carpenter on 07/11/2020.
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

set(LIBRARY_NAME Ribbon)

set(NEDRYSOFT_LIBRARY_RIBBON_EXPORT)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

set(library_SOURCES
    RibbonCheckBox.cpp
    RibbonCheckBox.h
    RibbonComboBox.cpp
    RibbonComboBox.h
    RibbonDropButton.cpp
    RibbonDropButton.h
    RibbonFontManager.cpp
    RibbonFontManager.h
    RibbonGroup.cpp
    RibbonGroup.h
    RibbonLineEdit.cpp
    RibbonLineEdit.h
    RibbonPushButton.cpp
    RibbonPushButton.h
    RibbonResources.qrc
    RibbonSlider.cpp
    RibbonSlider.h
    RibbonSpec.h
    RibbonTabBar.cpp
    RibbonTabBar.h
    RibbonToolButton.cpp
    RibbonToolButton.h
    RibbonToolButtonPlugin.cpp
    RibbonToolButtonPlugin.h
    RibbonWidget.cpp
    RibbonWidget.h
    ThemeSupport.h
    ThemeSupport.mm
)

add_library(${LIBRARY_NAME} SHARED
    ${library_SOURCES}
)

target_link_libraries(${LIBRARY_NAME} "-framework AppKit" "-framework  Cocoa")

target_link_libraries(${LIBRARY_NAME} -lstdc++ -lobjc)

target_link_libraries(${LIBRARY_NAME} "Qt5::Widgets")

set_target_properties(${LIBRARY_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${FRAMEWORK_OUTPUT}")
set_target_properties(${LIBRARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FRAMEWORK_OUTPUT}")

# designer widgets

option(Build_RibbonDesignerPlugin "Build Ribbon QtDesigner Plugin" OFF)

if(Build_RibbonDesignerPlugin)
    project(Nedrysoft.Ribbon)

    add_definitions(-DNEDRYSOFT_RIBBON_DESIGNER_EXPORT)

    find_package(Qt5 COMPONENTS Widgets UiPlugin REQUIRED)

    add_library(${PROJECT_NAME} SHARED
        ${library_SOURCES}
        RibbonCheckBoxPlugin.cpp
        RibbonCheckBoxPlugin.h
        RibbonComboBoxPlugin.cpp
        RibbonComboBoxPlugin.h
        RibbonDropButtonPlugin.cpp
        RibbonDropButtonPlugin.h
        RibbonGroupPlugin.cpp
        RibbonGroupPlugin.h
        RibbonLineEditPlugin.cpp
        RibbonLineEditPlugin.h
        RibbonPushButtonPlugin.cpp
        RibbonPushButtonPlugin.h
        RibbonWidgetsCollection.cpp
        RibbonWidgetsCollection.h
        RibbonWidgetPlugin.cpp
        RibbonWidgetPlugin.h
    )

    if(APPLE)
        target_sources(${PROJECT_NAME} "PRIVATE"
            ThemeSupport.mm
            ThemeSupport.h
            )
    endif()

    target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::UiPlugin)

    if(APPLE)
        target_link_libraries(${PROJECT_NAME} "-framework AppKit" "-framework Cocoa")
    endif()

    set(PLUGIN_OUTPUT_DIR ${APPLICATION_BIN_OUTPUT}/Designer/${CMAKE_LIBRARY_ARCHITECTURE}/${Qt5_VERSION})

    file(MAKE_DIRECTORY ${PLUGIN_OUTPUT_DIR})

    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}")
endif()
