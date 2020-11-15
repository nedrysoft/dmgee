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

#ifndef NEDRYSOFT_RIBBONDROPBUTTON_H
#define NEDRYSOFT_RIBBONDROPBUTTON_H

#include "RibbonSpec.h"
#include "ThemeSupport.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace Nedrysoft::Ribbon {
    constexpr auto RibbonDropButtonDefaultIconWidth = 32;
    constexpr auto RibbonDropButtonDefaultIconHeight = 48;
    constexpr auto RibbonDropButtonDefaultHeight = 16;
    constexpr auto RubbonDropButtonArrowWidth = 5;
    constexpr auto RubbonDropButtonArrowHeight = 5;


    /**
     * @brief       Ribbon Pushbutton with Drop down
     *
     * @details     A widget that provides a drop down menu for additional actions
     */
    class NEDRYSOFT_RIBBON_WIDGET_EXPORT RibbonDropButton :
        public QWidget {
            Q_OBJECT

            Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
            Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)

        public:
            /**
             * @brief       Constructs a RibbonDropButton
             *
             * @param[in]   parent is the owner widget
             */
            explicit RibbonDropButton(QWidget *parent=nullptr);

            /**
             * @brief       Destructor
             */
            ~RibbonDropButton() override;

            /**
             * @brief       Returns the current icon that is displayed on the main portion of the button
             * @return
             */
            QIcon icon();

            /**
             * @brief       Sets the icon to be displayed on the main portion of hhe button
             *
             * @param[in]   icon is the icon to be used
             */
            void setIcon(QIcon &icon);

            /**
             * @brief       Returns the current size of the icon
             *
             * @return      the size of the icon
             */
            QSize iconSize();

            /**
             * @brief       Sets the size of the main icon
             *
             * @param[in]   iconSize is the size of the icon
             */
            void setIconSize(QSize iconSize);

        private:
            /**
             * @brief       When the icon size is changed, this function is called to synchronise the widget sizes
             */
            void updateSizes();

            /**
             * @brief       Set the style sheet on sub-controls according to current light/dark mode
             *
             * @param[in]   isDarkMode true if dark mode; otherwise false
             */
            void updateStyleSheets(bool isDarkMode);

        public:
            /**
             * @brief       Signal that is emitted when the button is pressed
             *
             * @param[in]   dropdown is true if the drop down button was clicked; otherwise false (main button was clicked)
             */
            Q_SIGNAL void clicked(bool dropdown);

        private:
            QVBoxLayout *m_layout;                              //! the layout for this widget
            QPushButton *m_mainButton;                          //! the main button
            QPushButton *m_dropButton;                          //! the drop down button
            QSize m_iconSize;                                   //! the size of the icon
            Nedrysoft::Utils::ThemeSupport *m_themeSupport;     //! theme support instance
    };
}

#endif //NEDRYSOFT_RIBBONDROPBUTTON_H
