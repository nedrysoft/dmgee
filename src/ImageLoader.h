 /*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 30/10/2020.
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

#ifndef NEDRYSOFT_IMAGELOADER_H
#define NEDRYSOFT_IMAGELOADER_H

#include <QString>

 namespace Nedrysoft {
     /**
      * @brief      Provides functions for loading images natively on macOS, it uses NSImage to load the image
      *             or to obtain the image from a given filename or type (i.e the Applications folder image)
      */
     class ImageLoader {
         public:
             /**
             * @brief       loads an image via NSImage and returns a TIFF
             *
             * @details     NSImage is able to load various image types, due to a bug in the DevIL library that causes it to
             *              crash when loading a .icns file, we use this class as a first stage image loader.
             *
             *              If the image is loaded, then a tiff representation of the image is returned in the data parameter,
             *              ownership of the data is passed to the caller and they are responsible for freeing the allocated memory.
             *
             * @param[in]   filename is the file to be loaded
             * @param[out]  data is the binary data containing the TIFF image
             * @param[out]  length is the size of the TIFF image in bytes
             *
             * @returns     true if the image was loaded successfully; otherwise false.
             */
             static bool load(QString &filename, char **data, unsigned int *length);

             /**
             * @brief       returns a TIFF image of the files icon
             *
             * @param[in]   filename is the file whose icon is to be retrieved
             * @param[out]  data is the binary data containing the TIFF image
             * @param[out]  length is the size of the TIFF image in bytes
             * @param[in]   width the requested width of the image (images may have multiple sizes inside)
             * @param[in]   height the requested height of the image (images may have multiple sizes inside)
             *
             * @returns     true if the image was loaded successfully; otherwise false.
             */
             static bool imageForFile(QString &filename, char **data, unsigned int *length, int width, int height);
     };
 }

#endif //NEDRYSOFT_IMAGELOADER_H
