/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 19/11/2020.
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

#ifndef NEDRYSOFT_FLATTABBAR_H
#define NEDRYSOFT_FLATTABBAR_H

#include <QTabBar>

namespace Nedrysoft {
    /**
     * @brief       The FlatTabBar class in a flat style tab bar that matches the application style.
     */
    class FlatTabBar :
        public QTabBar {
            private:
                Q_OBJECT;

            protected:
                /**
                 * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
                 *
                 * @param[in]   event the event information.
                 */
                void paintEvent(QPaintEvent *event) override;
    };
};

#endif //NEDRYSOFT_FLATTABBAR_H
