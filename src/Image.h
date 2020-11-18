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
     * @brief       The Image class represents an image.
     *
     * @details     The loaded image is encapsulated in this class and the class provides different views of the image
     *              that are used by different parts of the application, currently the class provides access to the
     *              image as QImage, opencv::mat and a raw data image.
     */
    class Image {
        public:
            /**
             * @brief       Constructs a new Image instance.
             */
            explicit Image();

            /**
             * @brief       Constructs a new Image instance with the given parameters.
             *
             * @note        When requesting a thumbnail, the height and width paremeters are used as a hint as to
             *              the size of the image that is needed.
             *
             * @param[in]   filename the file to be loaded
             * @param[in]   loadContent true if loading an actual image; otherwise false to query the OS to get a thumbnail of the file.
             * @param[in]   height the requested image height if loadContent is false; otherwise ignored.
             * @param[in]   width the requested image width if loadContent is false; otherwise ignored.
             */
            explicit Image(QString filename, bool loadContent=true, int width=0, int height=0);

            /**
             * @brief       Destroys the Image.
             */
            ~Image();

            /**
             * @brief       Returns the width of the Image.
             *
             * @note        the width of the image may differ from the number of bytes per row, this may depend on the underlying
             *              type of image due to memory optimisations.
             *
             * @returns     the width of the image.
             */
            float width() const;

            /**
             * @brief       Returns the height of the Image.
             *
             * @returns     the height of the image.
             */
            float height() const;

            /**
             * @brief       Returns the stride of the image.
             *
             * @note        The stride is the number of bytes per row, due to optimisations this may differ from what
             *              is expected.  Do not assume that stride=width*components.
             *
             * @returns     the height of the image.
             */
            float stride() const;

            /**
             * @brief       Returns a pointer to the raw image data.
             *
             * @returns     the raw image pointer.
             */
            char *data();

            /**
             * @brief       Returns the image as a opencv mat.
             *
             * @returns     the opencv mat.
             */
            cv::Mat mat();

            /**
             * @brief       Returns the image as a QImage.
             *
             * @returns     the image.
             */
            QImage image();

            /**
             * @brief       Returns the raw image as QByteArray.
             *
             * @returns     the raw image data in a QByteArray.
             */
            QByteArray rawData();

            /**
             * @brief       Returns whether the image is valid.
             *
             * @note        After loading an image you should always check that the image is valid, reasons for it being
             *              invalid are unsupported image formats or a corrupted file.
             *
             * @returns     true if valid; otherwise false.
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
