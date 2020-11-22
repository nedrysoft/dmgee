/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 21/11/2020.
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

#ifndef NEDRYSOFT_MACHELPER_H
#define NEDRYSOFT_MACHELPER_H

#include <QPixmap>

namespace Nedrysoft {
    namespace StandardImage {
        /**
         * @brief       StandardImageName enum.
         *
         * @details     Provides parity naming with AppKit standard images.
         */
        enum StandardImageName {
            NSImageNamePreferencesGeneral,
            NSImageNameUserAccounts,
            NSImageNameAdvanced
        };
    };

    /**
     * @brief       The MacHelper class.
     *
     * @details     Provides helper functions to allow more native mac os functionality where required.
     */
    class MacHelper {
        public:
            /**
             * @brief       Returns a pixmap of a mac standard icon.
             *
             * @param[in]   imageName the standard image identifier.
             * @param[in]   size the required size of the resulting image.
             *
             * @returns     A pixmap of the named image.
             */
            static QPixmap macStandardImage(StandardImage::StandardImageName imageName, QSize imageSize);
    };
};

#endif // NEDRYSOFT_MACHELPER_H
