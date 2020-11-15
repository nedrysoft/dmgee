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

#ifndef NEDRYSOFT_RIBBONTABBAR_H
#define NEDRYSOFT_RIBBONTABBAR_H

#include "RibbonSpec.h"
#include "ThemeSupport.h"
#include <QFont>
#include <QTabBar>

namespace Nedrysoft::Ribbon {
    /**
     * @brief       Ribbon Tab Bar Widget
     *
     * @details     Subclass of QTabBar to create a lookalike of the Ribbon
     *              bar tab control.
     */
    class NEDRYSOFT_RIBBON_WIDGET_EXPORT RibbonTabBar :
        public QTabBar {
            private:
                Q_OBJECT

            public:
                /**
                 * @brief       Constructor
                 *
                 * @param[in]   parent is the parent widget to take ownership
                 */
                explicit RibbonTabBar(QWidget *parent = nullptr);

            protected:
                /**
                 * @brief       paintEvent
                 *
                 * @details     Overridden paintEvent for drawing widget
                 *
                 * @param[in]   event is the event information
                 */
                void paintEvent(QPaintEvent *event) override;

                /**
                 * @brief       tabSizeHint
                 *
                 * @details     Overridden paintEvent for size widget
                 *
                 * @param[in]   index is the index of the tab
                 *
                 * @return      the calculated size hint for the tab
                 */
                [[nodiscard]] QSize tabSizeHint(int index) const override;

                /**
                 * @brief       eventFilter
                 *
                 * @details     handles widget events passed from Qt
                 *
                 * @param[in]   obj is the object that caused the event
                 * @param[in]   event contains the event information
                 *
                 * @return      true if event has handled, otherwise false
                 */
                bool eventFilter(QObject * obj, QEvent * event) override;

            private:
                /**
                 * @brief       Sets the stylesheet for dark/light mode
                 *
                 * @param[in]   isDarkMode true if dark mode; otherwise false
                 */
                void updateStyleSheet(bool isDarkMode);

            private:
                QFont m_selectedFont;                                   //! font to use on selected tab
                QFont m_normalFont;                                     //! font to use on deselected tabs
                bool m_mouseInWidget;                                   //! tracks whether the mouse is tracked inside the widget

                Nedrysoft::Utils::ThemeSupport *m_themeSupport;         //! theme support instance
    };
}
#endif // NEDRYSOFT_RIBBONTABBAR_H
