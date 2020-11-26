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

#ifndef LICENCETEMPLATESSETTINGSPAGE_H
#define LICENCETEMPLATESSETTINGSPAGE_H

#include "ISettingsPage.h"

#include <QList>
#include <QMap>
#include <QStandardItemModel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "yaml-cpp/yaml.h"

namespace Ui {
    class LicenceTemplatesSettingsPage;
}

namespace Nedrysoft {
    /**
     * @brief               General settings page
     *
     * @details             A widget that implements ISettingsPage to provide configuration options for
     *                      database settings.
     */
    class LicenceTemplatesSettingsPage :
            public QWidget,
            public ISettingsPage
    {
            Q_OBJECT

        public:
            enum Type {
                Permissions,
                Conditions,
                Limitations,
            };

            struct Rule {
                QString m_description;
                QString m_label;
                QString m_tag;
            };

            struct Licence {
                QString m_title;
                QString m_spdxId;
                QString m_description;
                QString m_how;
                QMap<QString, QString> m_using;
                QStringList m_permissions;
                QStringList m_conditions;
                QStringList m_limitations;
                bool m_featured;
                bool m_hidden;
                QString m_nickname;
                QString m_note;
                QString m_redirectFrom;
                QString m_licence;
                bool m_valid;
            };

        public:
            /**
             * @brief       Constructs a new GeneralSettingsPage instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            explicit LicenceTemplatesSettingsPage(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the GeneralSettingsPage
             */
            ~LicenceTemplatesSettingsPage();

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

        private:
            /**
             * @brief       Loads a license file from "choosealicense.com"
             *
             * @details     The license file should have a YAML header which contains
             *              metadata about the license.
             *
             * @param[in]   filename the name of the file to load.
             *
             * @returns     the decoded license information.
             */
            Nedrysoft::LicenceTemplatesSettingsPage::Licence loadLicence(QString filename);

            /**
             * @brief       Populates the given layout with the list of strings
             *
             * @param[in]   type the identifier of list we are populating.
             * @param[in]   licence the license being processsed.
             */
            void createList(Nedrysoft::LicenceTemplatesSettingsPage::Type type, Nedrysoft::LicenceTemplatesSettingsPage::Licence licence);

            /**
             * @brief       Loads the rules YAML document.
             * @param[in]   filename the file name of the document.
             *
             * @returns     true if loaded; otherwise false.
             */
            bool loadRules(QString filename);

            /**
             * @brief       Imports the rules for the given type from the YAML document.
             *
             * @param[in]   document the YAML root node document.
             * @param[in]   type the type of the rule to import.
             *
             * @returns     the map of rule definitions for the given type.
             */
            QMap<QString, Nedrysoft::LicenceTemplatesSettingsPage::Rule> importRule(YAML::Node rulesDocument, Nedrysoft::LicenceTemplatesSettingsPage::Type type);

        protected:
            /**
             * @brief       Reimplements an access function for property: QWidget::sizeHint.
             *
             * @returns     the recommended size for the widget.
             */
            QSize sizeHint() const;

        private:
            Ui::LicenceTemplatesSettingsPage *ui;               //! ui class.

            QHBoxLayout *m_mainLayout;                          //! the main layout.
            QVBoxLayout *m_permissionsLayout;                   //! layout for permissions section.
            QVBoxLayout *m_conditionsLayout;                    //! layout for conditions section.
            QVBoxLayout *m_limitationsLayout;                   //! layout for limitations section.

            QMap<QString, Rule> m_conditionsRules;              //! The rules for conditions, human readable.
            QMap<QString, Rule> m_permissionsRules;             //! The rules for permissions, human readable.
            QMap<QString, Rule> m_limitationsRules;             //! The rules for limitations, human readable.

            QSize m_size;                                       //! the calculated size of the widget.

            QStandardItemModel m_licenceModel;                  //! the model that contains the license entries.
    };
}

Q_DECLARE_METATYPE(Nedrysoft::LicenceTemplatesSettingsPage::Licence);

#endif // LICENCETEMPLATESSETTINGSPAGE_H
