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

#include "Image.h"

#include "ImageLoader.h"

#include <IL/ilu.h>
#include <QBuffer>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <cmath>

Nedrysoft::Image::Image() :
        m_data(nullptr),
        m_width(0),
        m_height(0),
        m_stride(0),
        m_imageId(0),
        m_isValid(false),
        m_length(0) {
}

Nedrysoft::Image::Image(QString filename, bool loadContent, int width, int height) :
        m_data(nullptr),
        m_width(0),
        m_height(0),
        m_stride(0),
        m_imageId(0) {

    ILboolean success = IL_FALSE;
    char *tiffData;
    float scale = 1;
    auto expression = QRegularExpression(R"(@(?P<scale>(\d*))x\..*$)");
    const char *errorString;
    int errorOffset;
    int outputVectors[12];

    ilGenImages(1, &m_imageId);
    ilBindImage(m_imageId);

    // check if this is a retina image, if it is grab the scale factor from the filename

    auto match = expression.match(filename);

    if (match.hasMatch()) {
        scale = match.captured("scale").toFloat();
    }

    // DevIL seems to crash loading .icns files, so we try to use NSImage to load the requested image which returns a
    // TIFF representation of the file, we then load the TIFF file using DevIL.
    //
    // If NSImage fails to load the image then we fall back onto QImage, if QImage fails to load then we
    // fallback onto DevIL (and keep our fingers crossed)

    bool loadedData = false;
    unsigned int imageLength;

    if (loadContent) {
        loadedData = Nedrysoft::ImageLoader::load(filename, &tiffData, &imageLength);
    } else {
        loadedData = Nedrysoft::ImageLoader::imageForFile(filename, &tiffData, &imageLength, width, height);
    }

    if (loadedData) {
        success = ilLoadL(IL_TIF, tiffData, imageLength);

        free(tiffData);
    } else {
        QImage image;

        if (image.load(filename)) {
            QByteArray tiffData;
            QBuffer saveBuffer(&tiffData);

            saveBuffer.open(QIODevice::WriteOnly);

            image.save(&saveBuffer, "TIFF");

            saveBuffer.close();

            success = ilLoadL(IL_TIF, tiffData.data(), static_cast<unsigned int>(tiffData.length()));
        } else {
            success = ilLoadImage(filename.toLatin1());
        }
    }

    if (success == IL_TRUE) {
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

        if (success == IL_TRUE) {
            if (scale > 1) {
                iluScale(static_cast<int>(static_cast<float>(ilGetInteger(IL_IMAGE_WIDTH)) / scale),
                         static_cast<int>(static_cast<float>(ilGetInteger(IL_IMAGE_HEIGHT)) / scale),
                         static_cast<unsigned int>(ilGetInteger(IL_IMAGE_DEPTH)));
            }

            m_width = static_cast<unsigned int>(ilGetInteger(IL_IMAGE_WIDTH));
            m_height = static_cast<unsigned int>(ilGetInteger(IL_IMAGE_HEIGHT));
            m_data = reinterpret_cast<char *>(ilGetData());
            m_length = static_cast<unsigned int>(ilGetInteger(IL_IMAGE_SIZE_OF_DATA));
            m_stride =static_cast<unsigned int>(ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL)) * m_width;

            m_isValid = true;
        }
    }
}

Nedrysoft::Image::~Image() {
    if (m_data) {
        ilDeleteImages(1, &m_imageId);
    }
}

float Nedrysoft::Image::width() const {
    return static_cast<float>(m_width);
}

float Nedrysoft::Image::height() const {
    return static_cast<float>(m_height);
}

float Nedrysoft::Image::stride() const {
    return static_cast<float>(m_stride);
}

char *Nedrysoft::Image::data() {
    return m_data;
}

cv::Mat Nedrysoft::Image::mat() {
    auto mat = cv::Mat(cv::Size(m_width, m_height), CV_8UC4, m_data, cv::Mat::AUTO_STEP);

    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

    return mat;
}

QImage Nedrysoft::Image::image() {
    return QImage(reinterpret_cast<const uchar *>(m_data),
                  static_cast<int>(m_width),
                  static_cast<int>(m_height),
                  static_cast<int>(m_stride),
                  QImage::Format_RGBA8888);
}

QByteArray Nedrysoft::Image::rawData() {
    return QByteArray(QByteArray(static_cast<const char *>(m_data), m_length));
}

bool Nedrysoft::Image::isValid() const {
    return m_isValid;
}