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

#ifndef NEDRYSOFT_FLATTABWIDGET_H
#define NEDRYSOFT_FLATTABWIDGET_H

#include <QTabWidget>
#include "FlatTabBar.h"

namespace Nedrysoft {
    /**
     * @brief       The FlatTabWidget class is a flat style tab bar that uses FlatTabBar for the bar, this class
     *              simply replaces the tab bar with our custom one.  This has to be a subclass on QTabWidget as
     *              it is the only way to replace the tab bar.
     */
    class FlatTabWidget :
            public QTabWidget {

        public:
            /**
             * Constructs a new FlatTabWidget instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            FlatTabWidget(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the FlatTabWidget.
             */
            ~FlatTabWidget();

        private:
            Nedrysoft::FlatTabBar *m_tabBar;                            //! The customised tab bar
    };
};

#endif //NEDRYSOFT_FLATTABWIDGET_H
