#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
# An open source ping path analyser
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

add_library(${LIBRARY_NAME} SHARED
    ThemeSupport.mm
    ThemeSupport.h
    RibbonFontManager.cpp
    RibbonFontManager.h
    RibbonFontsResources.qrc
    RibbonGroup.cpp
    RibbonGroup.h
    RibbonTabBar.cpp
    RibbonTabBar.h
    RibbonWidget.cpp
    RibbonWidget.h
    RibbonSpec.h
)

target_link_libraries(${LIBRARY_NAME} "-framework AppKit" "-framework  Cocoa")

target_link_libraries(${LIBRARY_NAME} -lstdc++ -lobjc)

target_link_libraries(${LIBRARY_NAME} "Qt5::Widgets")

#set_target_properties(${LIBRARY_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${FRAMEWORK_OUTPUT}")
set_target_properties(${LIBRARY_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${FRAMEWORK_OUTPUT}")
set_target_properties(${LIBRARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FRAMEWORK_OUTPUT}")