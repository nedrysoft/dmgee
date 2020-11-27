/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 27/11/2020.
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

#ifndef NEDRYSOFT_HELPER_H
#define NEDRYSOFT_HELPER_H

#include <QPixmap>
#include <QtMacExtras>

namespace Nedrysoft {
    /**
     * @brief       The Helper class.
     *
     * @details     Provides helpful platform independant functions.
     */
    class Helper {
        public:
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
    };
};

#endif //NEDRYSOFT_HELPER_H
