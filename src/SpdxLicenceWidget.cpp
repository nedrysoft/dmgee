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

#include "SpdxLicenceWidget.h"

#include "ui_SpdxLicenceWidget.h"

#include <BulletWidget.h>
#include <QDebug>
#include <QFile>

Nedrysoft::SpdxLicenceWidget::SpdxLicenceWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SpdxLicenceWidget),
        m_licence(nullptr) {

    ui->setupUi(this);

    ui->licenseText->setFont(QFont("Fira Code", ui->licenseText->font().pointSize()));
}

Nedrysoft::SpdxLicenceWidget::SpdxLicenceWidget(Nedrysoft::SpdxLicence *licence) :
        SpdxLicenceWidget(static_cast<QWidget *>(nullptr)) {

    m_licence = licence;

    updateContent();
}

void Nedrysoft::SpdxLicenceWidget::updateContent(void) {
    ui->titleLabel->setText(m_licence->name());
    ui->licenseText->setText(m_licence->licenceText());

    int iconSize = 64;

    if (m_licence->isOsiApproved()) {
        ui->osiApprovedLabel->setText("OSI Approved");
        ui->osiApprovedIconLabel->setPixmap(QPixmap(":/icons/tick@2x.png").scaled(iconSize, iconSize));

    } else {
        ui->osiApprovedLabel->setText("Not OSI Approved");
        ui->osiApprovedIconLabel->setPixmap(QPixmap(":/icons/cross@2x.png").scaled(iconSize, iconSize));
    }
}