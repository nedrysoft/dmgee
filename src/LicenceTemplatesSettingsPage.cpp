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

#include "BulletWidget.h"
#include "ChooseALicenseLicence.h"
#include "MainWindow.h"
#include "SpdxLicence.h"
#include "ui_LicenceTemplatesSettingsPage.h"

#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QModelIndex>
#include <QUrl>

Nedrysoft::LicenceTemplatesSettingsPage::LicenceTemplatesSettingsPage(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LicenceTemplatesSettingsPage) {

    ui->setupUi(this);

    QStandardItem *chooseALicenseItem = new QStandardItem(tr("choosealicense.com"));
    QStandardItem *spdxItem = new QStandardItem(tr("spdx.org"));

    chooseALicenseItem->setEditable(false);
    spdxItem->setEditable(false);

    QDirIterator chooseALicenseIterator(":/choosealicence.com/_licenses", QDirIterator::Subdirectories);

    while (chooseALicenseIterator.hasNext()) {
        auto filename = chooseALicenseIterator.next();
        auto licence = new Nedrysoft::ChooseALicenseLicence(filename);

        if (licence->valid()) {
            auto item = new QStandardItem;

            if (!licence->nickname().isEmpty()) {
                item->setText(licence->nickname());
            } else {
                item->setText(licence->spdxId());
            }

            item->setData(QVariant::fromValue<Nedrysoft::ILicence *>(licence), Qt::UserRole);
            item->setEditable(false);

            chooseALicenseItem->appendRow(item);
        }
    }

    QDirIterator spdxIterator(":/spdx/json/details", QDirIterator::Subdirectories);

    while (spdxIterator.hasNext()) {
        auto filename = spdxIterator.next();
        auto licence = new Nedrysoft::SpdxLicence(filename);

        if (licence->valid()) {
            auto item = new QStandardItem;

            item->setText(licence->licenseId());

            item->setData(QVariant::fromValue<Nedrysoft::ILicence *>(licence), Qt::UserRole);
            item->setEditable(false);

            spdxItem->appendRow(item);
        }
    }

    m_licenceModel.appendRow(chooseALicenseItem);
    m_licenceModel.appendRow(spdxItem);

    ui->outlineView->setModel(&m_licenceModel);

    connect(ui->outlineView, &ThemedOutlineView::buttonClicked, [=](int buttonIndex) {
        // TODO: handle the buttons
    });

    connect(ui->outlineView, &ThemedOutlineView::clicked, [=](const QModelIndex &index) {
        ILicence *licence = nullptr;

        if ((!index.parent().isValid()) || (!index.isValid())) {
            return;
        }

        licence = index.data(Qt::UserRole).value<Nedrysoft::ILicence *>();

        if (ui->detailsWidget->currentIndex()!=0) {
            ui->detailsWidget->removeWidget(ui->detailsWidget->currentWidget());
        }

        auto licenceWidget = licence->widget();

        ui->detailsWidget->addWidget(licenceWidget);
        ui->detailsWidget->setCurrentWidget(licenceWidget);
    });

#if defined(Q_OS_MACOS)
    m_size = QSize(qMax(minimumSizeHint().width(), size().width()), qMax(minimumSizeHint().height(), size().height()));
#else
    m_size = minimumSizeHint();
#endif
}

Nedrysoft::LicenceTemplatesSettingsPage::~LicenceTemplatesSettingsPage() {
    delete ui;
}

bool Nedrysoft::LicenceTemplatesSettingsPage::canAcceptSettings() {
    return true;
}

void Nedrysoft::LicenceTemplatesSettingsPage::acceptSettings() {
}

QSize Nedrysoft::LicenceTemplatesSettingsPage::sizeHint() const {
    return m_size;
}
