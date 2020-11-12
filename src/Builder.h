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
#include <QMetaProperty>
#include <QPoint>

namespace Nedrysoft {
    /**
     * @brief       The builder class provides the serialisation of the configuration to/from TOML files and also
     *              uses the configuration to generate the DMG file
     */
    class Builder :
        public QObject {
            private:
                Q_OBJECT

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
                    QString name;                                   //! the name to display below the icon
                    QString shortcut;                               //! the target of tme symlink
                };

                /**
                 * @brief       Holds the configuration, this information is interchanged between this structure and a TOML format
                 *              configuration file.
                 */
                struct Configuration {
                    QString m_background;                           //! filename of the background image
                    QString m_icon;                                 //! icon to use when DMG is mounted
                    QString m_filename;                             //! output filename for the DMG
                    QString m_volumename;                           //! the name of the volume when mounted
                    int m_iconsize;                                 //! the size of the icons to be shown
                    QPoint m_gridSize;                              //! the grid spacing
                    bool m_snapToGrid;                              //! whether to snap points to grid
                    bool m_gridVisible;                             //! whether the grid is visible
                    int m_featureSize;                              //! minimum size in px^2 for feature detection
                    bool m_detectFeatures;                          //! whether we auto-detect features
                    QString m_format;                               //! format of the disk image

                    QList<Symlink> m_symlinks;                      //! list of symlinks in the DMG
                    QList<File> m_files;                            //! list of files in the DMG
                };

            public:
                /**
                 * @brief       Uses the dmgbuild python module + configuration file to being a DMG
                 *
                 * @param[in]   outputFilename is optionally the name of the file to create
                 *
                 * @returns     true on success; otherwise false
                 */
                bool createDMG(QString outputFilename=QString());

                /**
                 * @brief       Loads a configuration from disk into the m_configuration member
                 *
                 * @param[in]   filename is the name of the configuration to load
                 *
                 * @returns     true if the configuration loaded without error; otherwise false
                 */
                bool loadConfiguration(const QString& filename);

            public:
                Q_PROPERTY(QString background MEMBER (m_configuration.m_background));
                Q_PROPERTY(QString icon MEMBER (m_configuration.m_icon));
                Q_PROPERTY(QString filename MEMBER (m_configuration.m_filename));
                Q_PROPERTY(QString volumeName MEMBER (m_configuration.m_volumename));
                Q_PROPERTY(QString format MEMBER (m_configuration.m_format));
                Q_PROPERTY(int iconSize MEMBER (m_configuration.m_iconsize));
                Q_PROPERTY(QPoint gridSize MEMBER (m_configuration.m_gridSize));
                Q_PROPERTY(bool snapToGrid MEMBER (m_configuration.m_snapToGrid));
                Q_PROPERTY(bool gridVisible MEMBER (m_configuration.m_gridVisible));
                Q_PROPERTY(int featureSize MEMBER (m_configuration.m_featureSize));
                Q_PROPERTY(bool detectFeatures MEMBER (m_configuration.m_detectFeatures));

            private:
                Configuration m_configuration;                      //! the configuration for the DMG
    };
}

#endif //NEDRYSOFT_BUILDER_H
