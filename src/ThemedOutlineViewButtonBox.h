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

#ifndef NEDRYSOFT_THEMEDOUTLINEVIEWBUTTONBOX_H
#define NEDRYSOFT_THEMEDOUTLINEVIEWBUTTONBOX_H

#include <QWidget>

namespace Nedrysoft {
    /**
     * @brief       the ThemedOutlineViewButtonBox widget mimics a flat design button box consistent with macOS segmented control.
     */
    class ThemedOutlineViewButtonBox :
            public QWidget {

        private:
            Q_OBJECT;

        public:
            /**
             * @brief       Constructs a new ThemedOutlineViewButtonBox instance.
             */
            ThemedOutlineViewButtonBox(QWidget * parent = nullptr);

            /**
             * @brief       This signal is emitted when a button has been clicked
             *
             * @param[in]   buttonIndex the index of the button that was blicked.
             */
            Q_SIGNAL void buttonClicked(int buttonIndex);

        private:
            /**
             * @brief       Renders the widget content to a pixmap.
             *
             * @details     This widget draws to a pixmap to ensure that it draws without antialiasing, drawing
             *              directly in the paintEvent to the widget on macOS results in antialiasing which doesn't
             *              result in pixel perfect rendering.
             *
             * @param[in]   rect the rectangle of the widget.
             */
            void renderToPixmap(QRect rect);

            /**
             * @brief       Check if the button is highlighted.
             *
             * @param[in]   buttonIndex the index of the button to check.
             *
             * @returns     true if highlighted; otherwise false.
             */
            bool buttonIsHighlighted(int buttonIndex);

        protected:
            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @param[in]   event the event information.
             */
            void paintEvent(QPaintEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::mousePressEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            void mousePressEvent(QMouseEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::mouseReleaseEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            void mouseReleaseEvent(QMouseEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::mouseMoveEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            void mouseMoveEvent(QMouseEvent *event) override;

            /**
             * @brief       Reimplements an access function for property: QWidget::sizeHint.
             *
             * @returns     the recommended size for the widget.
             */
            QSize sizeHint();

            /**
             * @brief       Reimplements: QWidget::resizeEvent(QResizeEvent *event).
             *
             * @param[in]   event the event information.
             */
            void resizeEvent(QResizeEvent *event) override;

        private:
            QPixmap m_pixmap;                                       //! the widget pixmap
            int m_buttonIndex;                                      //! the current button index that is "hit"
    };
};

#endif //NEDRYSOFT_THEMEDOUTLINEVIEWBUTTONBOX_H
