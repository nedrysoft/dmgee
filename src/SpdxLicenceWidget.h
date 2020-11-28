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

#ifndef NEDRYSOFT_SPDXLICENSELICENCEWIDGET_H
#define NEDRYSOFT_SPDXLICENSELICENCEWIDGET_H

#include "SpdxLicence.h"
#include "Nedrysoft.h"
#include "yaml-cpp/yaml.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
    class SpdxLicenceWidget;
}

namespace Nedrysoft {
    /**
     * @brief       The SpdxLicence widget displays the detail information from a spdx licence.
     */
    class SpdxLicenceWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new SpdxLicenceWidget instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            explicit SpdxLicenceWidget(QWidget *parent = nullptr);

            /**
             * @brief       Constructs a new ChooseALicenseLicenceWidget instance for the license.
             *
             * @param[in]   licence the licence for be displayed.
             */
            explicit SpdxLicenceWidget(Nedrysoft::SpdxLicence *licence);

        private:

            /**
             * @brief       Updates the widget with the license data.
             */
            void updateContent(void);

        private:
            Ui::SpdxLicenceWidget *ui;                          //! ui class.

            QHBoxLayout *m_mainLayout;                          //! the main layout.

            Nedrysoft::SpdxLicence *m_licence;                  //! the license attached to this widget.
    };
}

#endif //NEDRYSOFT_SPDXLICENSELICENCEWIDGET_H
