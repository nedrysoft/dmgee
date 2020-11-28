/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 26/11/2020.
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

#include "ChooseALicenseLicence.h"

#include "ChooseALicenseLicenceWidget.h"
#include "Nedrysoft.h"

#include <QFile>
#include <QFileInfo>
#include <QMapIterator>
#include <QTextStream>
#include "yaml-cpp/yaml.h"

Nedrysoft::ChooseALicenseLicence::ChooseALicenseLicence() :
        m_valid(false) {

}

Nedrysoft::ChooseALicenseLicence::ChooseALicenseLicence(QString filename) {
    load(filename);
}

Nedrysoft::ChooseALicenseLicence::ChooseALicenseLicence(Nedrysoft::ChooseALicenseLicence &licence) {
    m_title = licence.title();
    m_spdxId = licence.spdxId();
    m_description = licence.description();
    m_how = licence.how();
    m_projectsUsing = licence.projectsUsing();
    m_permissions = licence.permissions();
    m_conditions = licence.conditions();
    m_limitations = licence.limitations();
    m_featured = licence.featured();
    m_hidden = licence.hidden();
    m_nickname = licence.nickname();
    m_note = licence.note();
    m_redirectFrom = licence.redirectFrom();
    m_licenceText = licence.licenceText();
    m_valid = licence.valid();
    m_filename = licence.filename();
}

QString Nedrysoft::ChooseALicenseLicence::licence(StringMap replacements) {
    QString licenceText = m_licenceText;

    auto mapIterator = StringMapIterator(replacements);

    while(mapIterator.hasNext()) {
        mapIterator.next();

        licenceText.replace("["+mapIterator.key()+"]", mapIterator.value());
    }

    return licenceText;
}

QString Nedrysoft::ChooseALicenseLicence::id() {
    return QStringLiteral("choosealicense.org");
}

QWidget *Nedrysoft::ChooseALicenseLicence::widget() {
    return new ChooseALicenseLicenceWidget(this);
}

bool Nedrysoft::ChooseALicenseLicence::load(QString filename) {
    QFile licenseFile(filename);
    QFileInfo fileInfo(filename);

    setValid(false);
    setFilename(fileInfo.fileName());

    if (licenseFile.open(QFile::ReadOnly)) {
        QTextStream inputStream(licenseFile.readAll());

        if (inputStream.readLine() == "---") {
            QString yamlDocument;

            while (!inputStream.atEnd()) {
                auto currentLine = inputStream.readLine();

                if (currentLine == "---") {
                    break;
                }

                yamlDocument.append(currentLine + "\r\n");
            }

            YAML::Node header = YAML::Load(yamlDocument.toStdString());

            if (header.IsNull()) {
                return false;
            }

            setLicenceText(inputStream.readAll());

            if (header["title"].IsScalar()) {
                setTitle(QString::fromStdString(header["title"].as<std::string>()));
            }

            if (header["spdx-id"].IsScalar()) {
                setSpdxId(QString::fromStdString(header["spdx-id"].as<std::string>()));
            }

            if (header["description"].IsScalar()) {
                setDescription(QString::fromStdString(header["description"].as<std::string>()));
            }

            if (header["how"].IsScalar()) {
                setHow(QString::fromStdString(header["how"].as<std::string>()));
            }

            if (header["permissions"].IsSequence()) {
                auto stdSequence = header["permissions"].as<std::list<std::string>>();
                QStringList permissionList;

                for (auto permission : stdSequence) {
                    permissionList.push_back(QString::fromStdString(permission));
                }

                setPermissions(permissionList);
            }

            if (header["conditions"].IsSequence()) {
                auto stdSequence = header["conditions"].as<std::list<std::string>>();
                QStringList conditionsList;

                for (auto condition : stdSequence) {
                    conditionsList.push_back(QString::fromStdString(condition));
                }

                setConditions(conditionsList);
            }

            if (header["limitations"].IsSequence()) {
                auto stdSequence = header["limitations"].as<std::list<std::string>>();
                QStringList limitationsList;

                for (auto limitation : stdSequence) {
                    limitationsList.push_back(QString::fromStdString(limitation));
                }

                setLimitations(limitationsList);
            }

            if (header["using"].IsMap()) {
                StringMap usingMap;
                auto stdMap = header["using"].as<std::map<std::string, std::string>>();

                for (auto project : stdMap) {
                    usingMap[QString::fromStdString(project.first)] = QString::fromStdString(project.second);
                }

                setProjectsUsing(usingMap);
            }

            if (header["featured"].IsScalar()) {
                setFeatured(QVariant::fromValue<QString>(QString::fromStdString(header["featured"].as<std::string>())).toBool());
            }

            if (header["hidden"].IsScalar()) {
                setHidden(QVariant::fromValue<QString>(
                        QString::fromStdString(header["hidden"].as<std::string>())).toBool());
            }

            if (header["nickname"].IsScalar()) {
                setNickname(QString::fromStdString(header["nickname"].as<std::string>()));
            }

            if (header["note"].IsScalar()) {
                setNote(QString::fromStdString(header["note"].as<std::string>()));
            }

            if (header["redirectfrom"].IsScalar()) {
                setRedirectFrom(QString::fromStdString(header["redirectfrom"].as<std::string>()));
            }

            setValid(true);

            return true;
        }
    }

    return false;
}
