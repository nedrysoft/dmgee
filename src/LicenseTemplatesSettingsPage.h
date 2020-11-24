/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 24/11/2020.
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

#ifndef LICENSETEMPLATESSETTINGSPAGE_H
#define LICENSETEMPLATESSETTINGSPAGE_H

#include <QWidget>
#include "ISettingsPage.h"

namespace Ui {
    class LicenseTemplatesSettingsPage;
}

namespace Nedrysoft {
    /**
     * @brief               General settings page
     *
     * @details             A widget that implements ISettingsPage to provide configuration options for
     *                      database settings.
     */
    class LicenseTemplatesSettingsPage :
            public QWidget,
            public ISettingsPage
    {
            Q_OBJECT

        public:
            /**
             * @brief               Constructor
             *
             * @details             Constructs a LicenseTemplatesSettingsPage instance
             *
             * @param[in]           parent is the parent widget (this is used to calculate the position of the window)
             */
            explicit LicenseTemplatesSettingsPage(QWidget *parent = nullptr);

            /**
             * @brief               Destructor
             *
             * @details             Destroys the instance
             */
            ~LicenseTemplatesSettingsPage();

            /**
             * @sa                  ISettingsPage::canAcceptSettings();
             */
            bool canAcceptSettings();

            /**
             * @sa                  ISettingsPage::acceptSettings();
             */
            void acceptSettings();

        protected:
            /**
             * @brief               Size hint
             *
             * @details             Provides a size hint for the widget.
             *
             * @returns             A QSize containting the size hint information
             */
            QSize sizeHint() const;

        private:
            Ui::LicenseTemplatesSettingsPage *ui;               //! ui class

            QSize m_size;                               //! the calculated size of the widget
    };
}

#endif // LICENSETEMPLATESSETTINGSPAGE_H
