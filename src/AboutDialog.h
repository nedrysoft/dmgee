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
     * @brief               AboutDialog class
     *
     * @details             A dialog box that shows details about the application.
     */
    class AboutDialog :
        public QDialog {
            private:
                Q_OBJECT

            public:
                /**
                 * @brief           Constructs an about dialog.
                 *
                 * @param[in]       parent is the the owner of the dialog.
                 */
                explicit AboutDialog(QWidget *parent = nullptr);

                /**
                 * @brief           Destroys the dialog.
                 */
                ~AboutDialog() override = default;

            protected:
                /**
                 * @brief           Handles events for the dialog box.
                 *
                 * @details         To allow the dialog box to be dismissed easily, event processing at a more
                 *                  granular level needs to occur.
                 *
                 * @param[in]       event contains information about the event being processed.
                 * @returns         true if the event was handled; otherwise, false.
                 */
                bool event(QEvent *event) override;

                /**
                 * @brief           Handles focus out events.
                 *
                 * @details         If the about dialog loses focus, this function is called to dismiss the dialog.
                 *
                 * @param[in]       event contains information about the focus event.
                 */
                void focusOutEvent(QFocusEvent *event) override;

                /**
                 * @brief           Renders the dialog.
                 *
                 * @details         Calls QDialog::paintEvent and then draws version information over the top in the correct place.
                 *
                 * @param[in]       event contains information about the paint event.
                 */
                void paintEvent(QPaintEvent *paintEvent) override;

            private:
                QPixmap m_backgroundPixmap;                      //! the background image used for the about dialog
    };
}
#endif // NEDRYSOFT_ABOUTDIALOG_H
