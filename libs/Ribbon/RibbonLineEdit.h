/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * https://github.com/fizzyade
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

#ifndef NEDRYSOFT_RIBBONLINEDIT_H
#define NEDRYSOFT_RIBBONLINEDIT_H

#include "RibbonSpec.h"
#include <QLineEdit>

namespace Nedrysoft::Ribbon {
    /**
     * @brief       Ribbon LineEdit
     *
     * @details     Flat style line edit subclass which is consitent with the ribbon style
     */
    class NEDRYSOFT_RIBBON_WIDGET_EXPORT RibbonLineEdit :
        public QLineEdit {
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a RibbonLineEdit
             *
             * @param[in]   parent is the owner widget
             */
            explicit RibbonLineEdit(QWidget *parent=nullptr);

            /**
             * @brief       Destructor
             */
            ~RibbonLineEdit() override;
    };
}

#endif //NEDRYSOFT_RIBBONLINEDIT_H
