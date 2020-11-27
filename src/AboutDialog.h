/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 30/10/2020.
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

#ifndef NEDRYSOFT_ABOUTDIALOG_H
#define NEDRYSOFT_ABOUTDIALOG_H

#include <QDialog>
#include <QPixmap>

namespace Nedrysoft {
    /**
     * @brief       The AboutDialog dialog provides information about the application.
     */
    class AboutDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new AboutDialog instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            explicit AboutDialog(QWidget *parent = nullptr);

        protected:
            /**
             * @brief       Reimplements: QObject::event(QEvent *e).
             *
             * @details     To allow the dialog box to be dismissed easily, event processing at a more
             *              granular level needs to occur.
             *
             * @param[in]   event contains information about the event being processed.
             *
             * @returns     true if the event was handled; otherwise, false.
             */
            bool event(QEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::focusOutEvent(QFocusEvent *event).
             *
             * @details     If the AboutDialog loses focus this function is called to dismiss the dialog.
             *
             * @param[in]   event the event information.
             */
            void focusOutEvent(QFocusEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @details     Overrides the default paintEvent and draws version information over the top in the correct place.
             *
             * @param[in]   event the event information.
             */
            void paintEvent(QPaintEvent *paintEvent) override;

        private:
            QPixmap m_backgroundPixmap;                      //! the background image used for the about dialog
    };
}
#endif // NEDRYSOFT_ABOUTDIALOG_H
