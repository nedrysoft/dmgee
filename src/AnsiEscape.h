/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 20/11/2020.
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

#ifndef NEDRYSOFT_ANSIESCAPE_H
#define NEDRYSOFT_ANSIESCAPE_H

#include <QString>
#include <QColor>

namespace Nedrysoft {
    /**
     * @brief       The standard ANSI colours
     */
    enum AnsiColour {
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,
        RESET = 39,

        LIGHTBLACK_EX = 90,
        LIGHTRED_EX = 91,
        LIGHTGREEN_EX = 92,
        LIGHTYELLOW_EX = 93,
        LIGHTBLUE_EX = 94,
        LIGHTMAGENTA_EX = 95,
        LIGHTCYAN_EX = 96,
        LIGHTWHITE_EX = 97,
    };

    /**
     * @brief       The standard ANSI styles.
     */
    enum AnsiStyle {
        BRIGHT = 1,                                 /**< Set foreground or background to light colour. */
        DIM = 2,                                    /**< Set foreground or background to dimmed colour. */
        NORMAL = 22,                                /**< Set foreground or background to normal colour. */
        RESET_ALL = 0                               /**< Reset ANSI state to default.. */
    };

    /**
     * @brief       The AnsiEscape class provides methods for generating the ANSI escape sequences, this allows the
     *              text to have colour and other attributes.
     */
    class AnsiEscape {
        public:
            /**
             * @brief       Returns the escape sequence to set the QColor as the foreground colour.
             *
             * @param[in]   colour the ANSI colour.
             *
             * @returns     The escape sequence.
             */
            static QString fore(QColor colour);

            /**
             * @brief       Returns the escape sequence to set the named color as the foreground colour.
             *
             * @param[in]   colour the ANSI colour.
             *
             * @returns     The escape sequence.
             */
            static QString fore(Nedrysoft::AnsiColour ansiColour);

            /**
             * @brief       Returns the escape sequence to set the color with the given "RGB" values as the foreground colour.
             *
             * @param[in]   r the red component.
             * @param[in]   g the green component.
             * @param[in]   b the blue component.
             *
             * @returns     The escape sequence.
             */
            static QString fore(int r, int g, int b);

            /**
             * @brief       Returns the escape sequence to set the QColor as the background colour.
             *
             * @param[in]   colour the ANSI colour.
             *
             * @returns     The escape sequence.
             */
            static QString back(QColor black);

            /**
             * @brief       Returns the escape sequence to set the named color as the background colour.
             *
             * @param[in]   colour the ANSI colour.
             *
             * @returns     The escape sequence.
             */
            static QString back(Nedrysoft::AnsiColour ansiColour);

            /**
             * @brief       Returns the escape sequence to set the color with the given "RGB" values as the background colour.
             *
             * @param[in]   r the red component.
             * @param[in]   g the green component.
             * @param[in]   b the blue component.
             *
             * @returns     The escape sequence.
             */
            static QString back(int r, int g, int b);

            /**
             * @brief       Returns the escape sequence to set the given style.
             *
             * @param[in]   style the style.
             *
             * @returns     The escape sequence.
             */
            static QString style(Nedrysoft::AnsiStyle style);

            /**
             * @brief       Returns the escape sequence to reset the colours & style.
             *
             * @returns     The escape sequence.
             */
            static QString reset();

            /**
             * @brief       Returns the escape sequence for a URL.
             *
             * @param[in]   url the URL that the link targets.
             * @param[in]   description the text that is visible to the user.
             *
             * @return      The escape sequence.
             */
            static QString link(QString url, QString description);

            /**
             * @brief       Returns the escape sequence for underline on or off.
             *
             * @param[in]   state true if underline; otherwise false;
             *
             * @return      The escape sequence.
             */
            static QString underline(bool state);
    };
}

#endif //NEDRYSOFT_ANSIESCAPE_H
