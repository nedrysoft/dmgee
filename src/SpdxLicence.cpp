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

#include "SpdxLicence.h"

#include "SpdxLicenceWidget.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include "yaml-cpp/yaml.h"

Nedrysoft::SpdxLicence::SpdxLicence() :
        m_valid(false) {

}

Nedrysoft::SpdxLicence::SpdxLicence(QString filename) {
    load(filename);
}

Nedrysoft::SpdxLicence::SpdxLicence(Nedrysoft::SpdxLicence &licence) {
    m_licenceText = licence.licenceText();
    m_valid = licence.valid();
    m_filename = licence.filename();
    m_isDeprecatedLicenseId = licence.isDeprecatedLicenseId();
    m_standardLicenseTemplate = licence.standardLicenseTemplate();
    m_name = licence.name();
    m_licenseId = licence.licenseId();
    m_isOsiApproved = licence.isOsiApproved();
}

QString Nedrysoft::SpdxLicence::licence(QStringMap replacements) {
    return QString();
}

QWidget *Nedrysoft::SpdxLicence::widget() {
    return new SpdxLicenceWidget(this);
}

bool Nedrysoft::SpdxLicence::load(QString filename) {
    QFile file(filename);

    if (file.open(QFile::ReadOnly)) {
        auto licenceFile = QJsonDocument::fromJson(file.readAll());

        if (licenceFile.isObject()) {
            auto jsonObject = licenceFile.object();

            setLicenceText(jsonObject.value("licenseText").toString());
            setFilename(filename);
            setIsDeprecatedLicenseId(jsonObject.value("isDeprecatedLicenseId").toBool());
            setStandardLicenseTemplate(jsonObject.value("standardLicenseTemplate").toString());
            setName(jsonObject.value("name").toString());
            setLicenseId(jsonObject.value("licenseId").toString());
            setIsOsiApproved(jsonObject.value("isOsiApproved").toBool());

            setValid(true);

            return true;
        }
    }

    return false;
}
