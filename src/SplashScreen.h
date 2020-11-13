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

#ifndef NEDRYSOFT_SPLASHSCREEN_H
#define NEDRYSOFT_SPLASHSCREEN_H

#include <QSplashScreen>

namespace Nedrysoft {
    /**
     * @brief               RegExSplashScreen class
     *
     * @details             A dialog box that provides a custom splash screen.
     */
    class SplashScreen :
        public QSplashScreen {
            private:
                Q_OBJECT

            private:
                /**
                 * @brief           Constructs a splash screen dialog.
                 */
                SplashScreen();

                /**
                 * @brief           Destructor for the splash screen dialog.
                 */
                ~SplashScreen() override;

                /**
                 * @brief`          Delete the copy constructor
                 */
                SplashScreen(const SplashScreen&) = delete;

                /**
                 * @brief`          Delete the copy constructor
                 */
                SplashScreen& operator=(const SplashScreen&) = delete;

            public:
                /**
                 * @brief           Get the splash screen singleton
                 *
                 * @return          The splash screen instance
                 */
                static SplashScreen *getInstance();

            protected:
                /**
                 * @brief           Draw contents of splash screen
                 *
                 * @details         Draws the splash screen, uses the default implementation and then overlays version
                 *                  information over the top
                 *
                 * @param[in]       painter is the painter to draw to.
                 */
                void drawContents(QPainter *painter) override;
    };
}

#endif // NEDRYSOFT_SPLASHSCREEN_H
