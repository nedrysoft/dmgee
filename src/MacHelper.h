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
#include <QtMacExtras>

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

    namespace AlertButton {
        /**
         * @brief       AlertButton enum.
         *
         * @details     Equivilent values from NSAlert
         */
        enum AlertButtonResult {
            FirstButton = 1000,
            SecondButton,
            ThirdButton
        };

        constexpr int Button(int n) {
            return FirstButton+n-1;
        };
    }

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

            /**
             * @brief       Returns a resolved path
             *
             * @notes       if the filename starts with ~, then it is resolved to the actual users home directory.
             *
             * @param[in]   filename the filename to be resp;ved.
             */
            static QString resolvedPath(QString filename);

            /**
             * @brief       Returns a normalized path.
             *
             * @notes       The filename may include ~, ., .., and will return the path to the given file.  If
             *              the file is in the users home directory then the path will be relative and start
             *              with ~/
             *
             * @param[in]   filename the filename to be normalized.
             */
            static QString normalizedPath(QString filename);

            /**
             * @brief       Shows a native macOS message box.
             * @param[in]   parent the owner widget.
             * @param[in]   messageText the main message text.
             * @param[in]   informativeText the text to appear below the main message.
             * @param[in]   buttons the list of buttons to appear.
             *
             * @note        macOS returns NSAlertFirstButtonReturn, NSAlertSecondButtonReturn, NSAlertThirdButtonReturn
             *              NSAlertFirstButtonReturn+n etc.  NSAlertFirstButtonReturn is the constant 1000.
             *
             * @returns     the id of the button which was selected.
             */
            static Nedrysoft::AlertButton::AlertButtonResult nativeAlert(QWidget *parent, QString messageText, QString informativeText, QStringList buttons);

            static void addSpacer(QMacToolBar *toolbar);
    };
};

#endif // NEDRYSOFT_MACHELPER_H
