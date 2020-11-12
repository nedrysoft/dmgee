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

#ifndef NEDRYSOFT_RIBBONBUTTON_H
#define NEDRYSOFT_RIBBONBUTTON_H

#include "RibbonSpec.h"
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>

namespace Nedrysoft::Ribbon {
    constexpr auto RibbonButtonDefaultIconWidth = 32;
    constexpr auto RibbonButtonDefaultIconHeight = 48;
    constexpr auto RibbonButtonDefaultFontSize = 10;

    /**
     * @brief       Ribbon Pushbutton with label
     *
     * @details     A widget that provides a bassic ribbon button
     */
    class NEDRYSOFT_RIBBON_WIDGET_EXPORT RibbonButton :
        public QWidget {
            Q_OBJECT

            Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
            Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
            Q_PROPERTY(QString text READ text WRITE setText)

        public:
            /**
             * @brief       Constructs a RibbonButton
             *
             * @param[in]   parent is the owner widget
             */
            explicit RibbonButton(QWidget *parent=nullptr);

            /**
             * @brief       Destructor
             */
            ~RibbonButton() override;

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

            /**
             * @brief       Returns the text on the button
             *
             * @return      the text on the button
             */
            QString text();

            /**
             * @brief       Sets the text on the button
             *
             * @param[in]   text is the text to display on the button
             */
            void setText(QString text);

        private:
            /**
             * @brief       When the icon size is changed, this function is called to synchronise the widget sizes
             */
            void updateSizes();

        signals:
            /**
             * @brief       Signal that is emitted when the button is pressed
             */
            void clicked();

        private:
            QVBoxLayout *m_layout;                          //! the layout for this widget
            QPushButton *m_mainButton;                      //! the main button
            QLabel *m_buttonLabel;                          //! the drop down button
            QSize m_iconSize;                               //! the size of the icon
    };
}

#endif //NEDRYSOFT_RIBBONBUTTON_H
