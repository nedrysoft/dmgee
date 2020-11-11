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
     * @brief   Provides methods to get the system font.
     */
    class Font {
        public:
            /**
             * @brief       Gets the name of the font used in the GUI of macOS
             *
             * @return      The name of the system font.
             */
            static QString systemFontName();

            /**
             * @brief       For the given font, this method returns the path where the font is stored.
             *
             * @param[in]   fontName is the name of the font to find
             *
             * @return      the path to the font if found; otherwise an empty string
             */
            static QString getFilename(const QString &fontName);
    };
};

#endif //NEDRYSOFT_FONT_H
