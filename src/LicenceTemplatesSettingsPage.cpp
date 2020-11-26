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

#include "LicenceTemplatesSettingsPage.h"
#include "MainWindow.h"
#include "ui_LicenceTemplatesSettingsPage.h"
#include "yaml-cpp/yaml.h"

#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QList>
#include <QMessageBox>

Nedrysoft::LicenseTemplatesSettingsPage::LicenseTemplatesSettingsPage(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LicenceTemplatesSettingsPage) {

    ui->setupUi(this);

    QDirIterator it(":/licences", QDirIterator::Subdirectories);

    QStandardItem *builtInLicenses = new QStandardItem("Built In");

    while (it.hasNext()) {
        auto filename = it.next();
        auto licence = loadLicence(filename);

        if (licence.m_valid) {
            auto item = new QStandardItem;

            item->setText(licence.m_spdxId);

            builtInLicenses->appendRow(item);
        }
    }

    QDirIterator it2(":", QDirIterator::Subdirectories);
while (it2.hasNext()) {
    qDebug() << it2.next();
}

    m_licenceModel.appendRow(builtInLicenses);

    ui->widget->setItemModel(&m_licenceModel);

#if defined(Q_OS_MACOS)
    m_size = QSize(qMax(minimumSizeHint().width(), size().width()), qMax(minimumSizeHint().height(), size().height()));
#else
    m_size = minimumSizeHint();
#endif
}

Nedrysoft::LicenseTemplatesSettingsPage::~LicenseTemplatesSettingsPage()
{
    delete ui;
}

bool Nedrysoft::LicenseTemplatesSettingsPage::canAcceptSettings()
{
    return true;
}

void Nedrysoft::LicenseTemplatesSettingsPage::acceptSettings()
{
}

QSize Nedrysoft::LicenseTemplatesSettingsPage::sizeHint() const
{
    return m_size;
}

Nedrysoft::LicenseTemplatesSettingsPage::Licence Nedrysoft::LicenseTemplatesSettingsPage::loadLicence(QString filename) {
    Nedrysoft::LicenseTemplatesSettingsPage::Licence licence;
    QFile licenseFile(filename);

    licence.m_valid = false;

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
                return licence;
            }

            licence.m_licence = inputStream.readAll();

            if (header["title"].IsScalar()) {
                licence.m_title = QString::fromStdString(header["title"].as<std::string>());
            }

            if (header["spdx-id"].IsScalar()) {
                licence.m_spdxId = QString::fromStdString(header["spdx-id"].as<std::string>());
            }

            if (header["description"].IsScalar()) {
                licence.m_description = QString::fromStdString(header["description"].as<std::string>());
            }

            if (header["how"].IsScalar()) {
                licence.m_how = QString::fromStdString(header["how"].as<std::string>());
            }

            if (header["permissions"].IsSequence()) {
                auto stdSequence = header["permissions"].as<std::list<std::string>>();

                for (auto permission : stdSequence) {
                    licence.m_permissions.push_back(QString::fromStdString(permission));
                }
            }

            if (header["conditions"].IsSequence()) {
                auto stdSequence = header["conditions"].as<std::list<std::string>>();

                for (auto condition : stdSequence) {
                    licence.m_conditions.push_back(QString::fromStdString(condition));
                }
            }

            if (header["limitations"].IsSequence()) {
                auto stdSequence = header["limitations"].as<std::list<std::string>>();

                for (auto limitation : stdSequence) {
                    licence.m_limitations.push_back(QString::fromStdString(limitation));
                }
            }

            if (header["using"].IsMap()) {
                auto stdMap = header["using"].as<std::map<std::string, std::string>>();

                for (auto project : stdMap) {
                    licence.m_using[QString::fromStdString(project.first)] = QString::fromStdString(project.second);
                }
            }

            if (header["featured"].IsScalar()) {
                licence.m_featured = QVariant::fromValue<QString>(QString::fromStdString(header["featured"].as<std::string>())).toBool();
            }

            if (header["hidden"].IsScalar()) {
                licence.m_hidden = QVariant::fromValue<QString>(
                        QString::fromStdString(header["hidden"].as<std::string>())).toBool();
            }

            if (header["nickname"].IsScalar()) {
                licence.m_nickname = QString::fromStdString(header["nickname"].as<std::string>());
            }

            if (header["note"].IsScalar()) {
                licence.m_note = QString::fromStdString(header["note"].as<std::string>());
            }

            if (header["redirectfrom"].IsScalar()) {
                licence.m_redirectFrom = QString::fromStdString(header["redirectfrom"].as<std::string>());
            }

            licence.m_valid = true;
        }
    }

    return licence;
}