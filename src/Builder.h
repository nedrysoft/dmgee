/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 07/11/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NEDRYSOFT_BUILDER_H
#define NEDRYSOFT_BUILDER_H

#include "tomlplusplus/toml.hpp"
#include <QList>
#include <QString>
#include <fstream>

namespace Nedrysoft {
    /**
     * @brief       The builder class provides the serialisation of the configuration to/from TOML files and also
     *              uses the configuration to generate the DMG file
     */
    class Builder {
        private:
            /**
             * @brief       Holds the information on files that are contained in the DMG, the x, y coordinates are
             *              relative to the DMG background
             */
            struct File {
                int x;                                          //! x coordinate of the file
                int y;                                          //! y coordinate of the file
                QString file;                                   //! the file name this file. (can be absolute, but recommended to be relative to this config
            };

            /**
             * @brief       Holds the information on aliases/links that are contained in the DMG, the x, y coordinates are
             *              relative to the DMG background and the symlink is a link to a folder that exists on the destination.
             */
            struct Symlink {
                int x;                                          //! x coordinate of the symlink
                int y;                                          //! y coordinate of the symlink
                QString mame;                                   //! the name to display below the icon
                QString shortcut;                               //! the target of tme symlink
            };

            /**
             * @brief       Holds the configuration, this information is interchanged between this structure and a TOML format
             *              configuration file.
             */
            struct Configuration {
                QString background;                             //! filename of the background image
                QString icon;                                   //! icon to use when DMG is mounted
                QString filename;                               //! output filename for the DMG
                QString volumename;                             //! the name of the volume when mounted
                int iconsize;                                   //! the size of the icons to be shown

                QList<Symlink> symlinks;                        //! list of symlinks in the DMG
                QList<File> files;                              //! list of files in the DMG
            };

        public:
            /**
             * @brief       Uses the dmgbuild pyython module + configuration file to being a DMG
             *
             * @param[in]   table is the configuration in toml format
             *
             * @returns     true on success; otherwise false
             */
            bool createDMG(toml::table table);

            /**
             * @brief       Loads a configuration from disk into the m_configurtation member
             *
             * @param[in]   filename is the name of the configuration to load
             *
             * @returns     true if the configuration loaded without error; otherwise false
             */
            bool loadConfiguration(QString filename);

        private:
            Configuration m_configuration;                      //! the configuration for the DMG
    };
}

#endif //NEDRYSOFT_BUILDER_H
