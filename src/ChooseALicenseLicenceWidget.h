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

#ifndef NEDRYSOFT_CHOOSEALICENSELICENCEWIDGET_H
#define NEDRYSOFT_CHOOSEALICENSELICENCEWIDGET_H

#include "ChooseALicenseLicence.h"
#include "Nedrysoft.h"
#include "yaml-cpp/yaml.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
    class ChooseALicenseLicenceWidget;
}

namespace Nedrysoft {
    /**
     * @brief       The AboutDialog dialog provides information about the application.
     */
    class ChooseALicenseLicenceWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       The attributes that are attached to the licence.
             */
            enum Type {
                Permissions,
                Conditions,
                Limitations,
            };

            /**
             * @brief       Types of data that exist in the licence data
             */
            class Rule {
                NEDRY_PROPERTY(QString, description, description, setDescription);
                NEDRY_PROPERTY(QString, label, label, setLabel);
                NEDRY_PROPERTY(QString, tag, tag, setTag);
            };

        public:
            /**
             * @brief       Constructs a new ChooseALicenseLicenceWidget instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            explicit ChooseALicenseLicenceWidget(QWidget *parent = nullptr);

            /**
             * @brief       Constructs a new ChooseALicenseLicenceWidget instance for the license.
             *
             * @param[in]   licence the licence for be displayed.
             */
            explicit ChooseALicenseLicenceWidget(Nedrysoft::ChooseALicenseLicence *licence);

        private:
            /**
             * @brief       Imports and parses the rules document which provides extra information.
             *
             * @param[in]   rulesDocument the YAML document object.
             * @param[in]   type the type of attributes to read.
             *
             * @returns     the rule map.
             */
            QMap<QString, Rule> importRule(YAML::Node rulesDocument, Type type);

            /**
             * @brief       Creates and inserts in the widget the specified attribute types.
             *
             * @param[in]   type the type of attribute to process.
             */
            void createAttributeWidgets(Type type);

            /**
             * @brief       Loads the rule YAML which provides more detail to the user.
             *
             * @param[in]   filename the filename of the document.
             *
             * @returns     true if loaded; otherwise false.
             */
            bool loadRules(QString filename);

            /**
             * @brief       Updates the widget with the license data.
             */
            void updateContent(void);

        private:
            Ui::ChooseALicenseLicenceWidget *ui;                //! ui class.

            QHBoxLayout *m_mainLayout;                          //! the main layout.
            QVBoxLayout *m_permissionsLayout;                   //! layout for permissions section.
            QVBoxLayout *m_conditionsLayout;                    //! layout for conditions section.
            QVBoxLayout *m_limitationsLayout;                   //! layout for limitations section.

            QMap<QString, Rule> m_conditionsRules;              //! The rules for conditions, human readable.
            QMap<QString, Rule> m_permissionsRules;             //! The rules for permissions, human readable.
            QMap<QString, Rule> m_limitationsRules;             //! The rules for limitations, human readable.

            Nedrysoft::ChooseALicenseLicence *m_licence;        //! the license attached to this widget.
    };
}

#endif //NEDRYSOFT_CHOOSEALICENSELICENCEWIDGET_H
