/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 18/11/2020.
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

#ifndef NEDRYSOFT_ILICENCE_H
#define NEDRYSOFT_ILICENCE_H

#include <QString>

namespace Nedrysoft {
    /**
     * @brief       The Interface definition for a license.
     *
     * @details     Describes the interface contract that a license must adhere to.
     */
    class ILicense {
        public:
            /**
             * @brief       Returns the license text for this licence.
             *
             * @param[in]   replacements the map containing any field values that should be replaced in the licence text.
             *
             * @returns     A licence as a string.
             */
            virtual QString license(QMap<QString, QString> replacements) = 0;
    };
}

#endif // NEDRYSOFT_ILICENCE_H