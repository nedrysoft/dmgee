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

#ifndef NEDRYSOFT_FONT_H
#define NEDRYSOFT_FONT_H

#include <QString>

namespace Nedrysoft {
    /**
     * @brief       The Font class provides access to the system font.
     *
     * @details     Font provides the the application with a means to determine the UI font in macOS.  This is required
     *              so that text can be drawn over the background image to show the filename on icons in the preview window.
     */
    class Font {
        public:
            /**
             * @brief       Returns the name of the font used in the GUI of macOS.
             *
             * @returns     the name of the system font.
             */
            static QString systemFontName();

            /**
             * @brief       Returns the path of the font specified by fontName.
             *
             * @param[in]   fontName is the name of the font to find.
             *
             * @returns     the path to the font if found; otherwise an empty string.
             */
            static QString getFilename(const QString &fontName);
    };
};

#endif //NEDRYSOFT_FONT_H
