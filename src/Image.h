 /*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 29/10/2020.
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

#ifndef NEDRYSOFT_IMAGE_H
#define NEDRYSOFT_IMAGE_H

#include "IL/il.h"

#include <QImage>
#include <QString>
#include <opencv2/opencv.hpp>

namespace Nedrysoft {
    /**
     * @brief       Represents an image and provides multiple representations of it as cairo, opencv and ImGui texture
     *              to access it.
     */
    class Image {
        public:
            /**
             * @brief       constructs an Image object
             */
            explicit Image();

            /**
             * @brief       constructs an Image object
             *
             * @param[in]   filename is the file to be loaded
             * @param[in]   loadContent true if loading an actual image; otherwise false if getting the icon for the file
             * @param[in]   height of the requested image if loadContent is false; otherwise ignored.
             * @param[in]   width of the requested image if loadContent is false; otherwise ignored.
             *
             */
            explicit Image(QString filename, bool loadContent=true, int width=0, int height=0);

            /**
             * @brief       destroys the Image object
             *
             */
            ~Image();

            /**
             * @brief       returns the width of the Image
             *
             * @note        the width of the image may differ from the number of bytes per row, this may depend on the underlying
             *              type of image due to memory optimisations.
             *
             * @return      the width of the image
             */
            float width() const;

            /**
             * @brief       returns the height of the Image
             *
             * @return      the height of the image
             */
            float height() const;

            /**
             * @brief       returns the stride of the image (may differ from width*components for optimisation)
             *
             * @return      the height of the image
             */
            float stride() const;

            /**
             * @brief       returns the stride of the image (may differ from width*components for optimisation)
             *
             * @return      the height of the image
             */
            char *data();

            /**
             * @brief       returns the image as a opencv image
             *
             * @return      the image mat
             */
            cv::Mat mat();

            /**
             * @brief       returns a QImage of the image data
             *
             * @return      the image
             */
             QImage image();

            /**
             * @brief       returns the raw image as a byte array
             *
             * @return      the bytearray containing the pixel data
             */
            QByteArray rawData();

            /**
             * @brief       returns whether the image is valid
             *
             * @return      true if valid; otherwise false
             */
            bool isValid() const;

        private:
            char *m_data;                           //! the raw image data
            int m_length;                           //! the length in bytes of the decoded image data

            unsigned int m_width;                   //! the width of the image
            unsigned int m_height;                  //! the height of the image
            unsigned int m_stride;                  //! the stride of the backing image

            bool m_isValid;                         //! whether an valid image has been loaded

            ILuint m_imageId;                       //! the DevIL image identifier
    };
};

#endif //NEDRYSOFT_IMAGE_H
