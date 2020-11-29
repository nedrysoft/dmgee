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

#ifndef NEDRYSOFT_CHOOSEALICENSELICENCE_H
#define NEDRYSOFT_CHOOSEALICENSELICENCE_H

#include "ILicence.h"
#include "Nedrysoft.h"

#include <QMetaType>

namespace Nedrysoft {
    /**
     * @brief       The ChooseALicenseLicence class holds decoded data from choosealicense.com and
     *              provides a ILicence interface for using it.
     */
    class ChooseALicenseLicence :
            public ILicence {

        public:
            /**
             * @brief       Constructs a new ChooseALicenseLicence.
             */
            ChooseALicenseLicence();

            /**
             * @brief       Constructs a new ChooseALicenseLicence from the content of the file.
             *
             * @param[in]   filename the filename of the license to load.
             */
            ChooseALicenseLicence(QString filename);

             /**
             * @brief       Constructs a new ChooseALicenseLicence from an existing license.
             *
             * @param[in]   licence the licence to copy
             */
            ChooseALicenseLicence(ChooseALicenseLicence &licence);

            /**
             * @brief       Constructs a copy of other.
             */
            ChooseALicenseLicence(const Nedrysoft::ChooseALicenseLicence &other) = default;

            /**
             * @brief       Assigns other to this licence and returns a reference to this licence..
             */
            Nedrysoft::ChooseALicenseLicence & operator=(const Nedrysoft::ChooseALicenseLicence &other) = default;

            /**
             * @brief       Loads the license file.
             *
             * @param[in]   filename the filename to load.
             *
             * @returns     true if the file was loaded; otherwise false.
             */
            bool load(QString filename);

            /**
             * @brief       Reimplements: ILicense::license(StringMap replacements)
             *
             * @param[in]   replacements the replacements contains a map of variables to replace in the licence text.
             *
             * @returns     the license text.
             */
            QString licence(StringMap replacements) override;

            /**
             * @brief       Reimplements: ILicense::id()
             *
             * @returns     the identifier for this license.
             */
            QString id() override;

             /**
             * @brief       Reimplements: ILicense::widget()
             *
             * @returns     the viewer widget.
             */
            QWidget *widget() override;

            /**
             * @defgroup       Properties       the properties that describe a choosealicense.com data file.
             *
             * @{
             */
            NEDRY_PROPERTY(QString, title, title, setTitle);
            NEDRY_PROPERTY(QString, spdxId, spdxId, setSpdxId);
            NEDRY_PROPERTY(QString, description, description, setDescription);
            NEDRY_PROPERTY(QString, how, how, setHow);
            NEDRY_PROPERTY(StringMap, projectsUsing, projectsUsing, setProjectsUsing);
            NEDRY_PROPERTY(QStringList, permissions, permissions, setPermissions);
            NEDRY_PROPERTY(QStringList, conditions, conditions, setConditions);
            NEDRY_PROPERTY(QStringList, limitations, limitations, setLimitations);
            NEDRY_PROPERTY(bool, featured, featured, setFeatured);
            NEDRY_PROPERTY(bool, hidden, hidden, setHidden);
            NEDRY_PROPERTY(QString, nickname, nickname, setNickname);
            NEDRY_PROPERTY(QString, note, note, setNote);
            NEDRY_PROPERTY(QString, redirectFrom, redirectFrom, setRedirectFrom);
            NEDRY_PROPERTY(QString, licenceText, licenceText, setLicenceText);
            NEDRY_PROPERTY(bool, valid, valid, setValid);
            NEDRY_PROPERTY(QString, filename, filename, setFilename);
            /**
             * @}
             */
    };
}

#endif //NEDRYSOFT_CHOOSEALICENSELICENCE_H
