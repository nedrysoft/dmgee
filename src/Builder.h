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
#include <QMetaProperty>
#include <QPoint>
#include <QString>
#include <fstream>

#include <Python.h>

namespace Nedrysoft {
    /**
     * @brief       The Builder class is capable of loading and saving configurations, it also provides the function
     *              to create the DMG based on the current configuration.
     */
    class Builder :
        public QObject {
            private:
                Q_OBJECT

            public:
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
                 * @brief       Sets the text position relative to the icon
                 */
                enum TextPosition {
                    Bottom = 0,                                     /**< Text is located below the icon. */
                    Right = 1                                       /**< Text is located to the right of the icon. */
                };

            private:
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
                    bool m_snapToFeatures;                          //! whether to snap to features
                    int m_featureSize;                              //! minimum size in px^2 for feature detection
                    bool m_detectFeatures;                          //! whether we auto-detect features
                    bool m_iconsVisible;                            //! whether icons are displayed on the preview
                    QString m_format;                               //! format of the disk image
                    int m_textSize;                                 //! size of the icon text in points
                    TextPosition m_textPosition;                    //! the position of the text relative to the icon

                    QList<Symlink *> m_symlinks;                    //! list of symlinks in the DMG
                    QList<File *> m_files;                          //! list of files in the DMG
                };

            public:
                /**
                 * @brief       Uses the dmgbuild python module + configuration file to being a DMG.
                 *
                 * @param[in]   outputFilename the output name of the file to create (or empty to use the value in the configuration).
                 *
                 * @returns     true on success; otherwise false.
                 */
                bool createDMG(QString outputFilename=QString());

                /**
                 * @brief       Loads a configuration from a file.
                 *
                 * @param[in]   filename the name of the configuration to load.
                 *
                 * @returns     true if loaded without error; otherwise false.
                 */
                bool loadConfiguration(const QString& filename);

            private:
                /**
                 * @brief       Python function which allows transfer of a string to c
                 *
                 * @param[in]   self the python object
                 * @param[in]   updateData string containing the data being transferred
                 *
                 * @returns     PyTrue if handled; other PyFalse
                 */
                static PyObject *update(PyObject *self, PyObject *updateData);

            public:
                /**
                 * @brief       This signal is emitted when the python script progress is updated.
                 *
                 * @param[in]   progress the json progress update as a QString
                 */
                Q_SIGNAL void progressUpdate(QString progress);

            private:
                /**
                 * @brief       Sets the list of symlinks to be added to the DMG.
                 *
                 * @param[in]   symlinks the list of symlinks.
                 */
                void setSymlinks(QList<Symlink *> symlinks);

                /**
                 * @brief       Returns the list of symlinks.
                 *
                 * @returns     the symlinks list.
                 */
                QList<Symlink *> symlinks();

                /**
                 * @brief       Sets the list of files to be added to the DMG.
                 *
                 * @param[in]   files the list of files.
                 */
                void setFiles(QList<File *> files);

                /**
                 * @brief       Returns the list of files.
                 *
                 * @returns     the file list.
                 */
                QList<File *> files();

            public:
                Q_PROPERTY(QString background MEMBER (m_configuration.m_background));
                Q_PROPERTY(QString icon MEMBER (m_configuration.m_icon));
                Q_PROPERTY(QString filename MEMBER (m_configuration.m_filename));
                Q_PROPERTY(QString volumeName MEMBER (m_configuration.m_volumename));
                Q_PROPERTY(QString format MEMBER (m_configuration.m_format));
                Q_PROPERTY(int iconSize MEMBER (m_configuration.m_iconsize));
                Q_PROPERTY(QPoint gridSize MEMBER (m_configuration.m_gridSize));
                Q_PROPERTY(bool snapToGrid MEMBER (m_configuration.m_snapToGrid));
                Q_PROPERTY(bool snapToFeatures MEMBER (m_configuration.m_snapToFeatures));
                Q_PROPERTY(bool gridVisible MEMBER (m_configuration.m_gridVisible));
                Q_PROPERTY(bool iconsVisible MEMBER (m_configuration.m_iconsVisible));
                Q_PROPERTY(int featureSize MEMBER (m_configuration.m_featureSize));
                Q_PROPERTY(bool detectFeatures MEMBER (m_configuration.m_detectFeatures));
                Q_PROPERTY(QList<Nedrysoft::Builder::Symlink *> symlinks READ symlinks WRITE setSymlinks);
                Q_PROPERTY(QList<Nedrysoft::Builder::File *> files READ files WRITE setFiles);
                Q_PROPERTY(int textSize MEMBER (m_configuration.m_textSize));
                Q_PROPERTY(Nedrysoft::Builder::TextPosition textPosition MEMBER (m_configuration.m_textPosition));

            private:
                Configuration m_configuration;                      //! the configuration for the DMG

                static PyMethodDef m_moduleMethods[];               //! module method table for the dmgee module
    };
}

Q_DECLARE_METATYPE(Nedrysoft::Builder::File *);
Q_DECLARE_METATYPE(Nedrysoft::Builder::Symlink *);
Q_DECLARE_METATYPE(Nedrysoft::Builder::TextPosition);

#endif //NEDRYSOFT_BUILDER_H
