/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 21/11/2020.
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

#ifndef NEDRYSOFT_USERSETTINGSPAGE_H
#define NEDRYSOFT_USERSETTINGSPAGE_H

#include "ISettingsPage.h"

#include <QWidget>

namespace Ui {
    class UserSettingsPage;
}

namespace Nedrysoft {
    /**
     * @brief       The UserSettingsPage class.
     *
     * @details     A widget that implements ISettingsPage to provide the configuration editor
     *              for user settings;
     */
    class UserSettingsPage :
            public QWidget,
            public ISettingsPage {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new UserSettingsPage instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            explicit UserSettingsPage(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the UserSettingsPage
             */
            ~UserSettingsPage();

            /**
             * @brief       Reimplements: ISettingsPage::canAcceptSettings().
             *
             * @returns     true if settings can be accepted; otherwise false.
             */
            bool canAcceptSettings();

            /**
             * @brief       Reimplements: ISettingsPage::acceptSettings().
             */
            void acceptSettings();

        protected:
            /**
             * @brief       Reimplements an access function for property: QWidget::sizeHint.
             *
             * @returns     the recommended size for the widget.
             */
            QSize sizeHint() const;

        private:
            Ui::UserSettingsPage *ui;                   //! ui class

            QSize m_size;                               //! the calculated size of the widget
    };
}

#endif // NEDRYSOFT_USERSETTINGSPAGE_H
