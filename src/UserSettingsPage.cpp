/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 21/11/2020.
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

#include "UserSettingsPage.h"

#include "SettingsManager.h"
#include "ui_UserSettingsPage.h"

#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QShowEvent>

Nedrysoft::UserSettingsPage::UserSettingsPage(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::UserSettingsPage) {

    ui->setupUi(this);

#if defined(Q_OS_MACOS)
    m_size = QSize(qMax(minimumSizeHint().width(), size().width()), minimumSizeHint().height());
#else
    m_size = minimumSizeHint();
#endif
    Nedrysoft::SettingsManager settingsManager;

    ui->fullNameLineEdit->setText(settingsManager.fullname());
    ui->userNameLineEdit->setText(settingsManager.username());
    ui->emailAddressLineEdit->setText(settingsManager.email());

    connect(ui->fullNameLineEdit, &QLineEdit::textEdited, [this](const QString &text) {
        Nedrysoft::SettingsManager settingsManager;

        settingsManager.setFullname(text);
    });

    connect(ui->userNameLineEdit, &QLineEdit::textEdited, [this](const QString &text) {
        Nedrysoft::SettingsManager settingsManager;

        settingsManager.setUsername(text);
    });

    connect(ui->emailAddressLineEdit, &QLineEdit::textEdited, [this](const QString &text) {
        Nedrysoft::SettingsManager settingsManager;

        settingsManager.setEmail(text);
    });
}

Nedrysoft::UserSettingsPage::~UserSettingsPage() {
    delete ui;
}

bool Nedrysoft::UserSettingsPage::canAcceptSettings() {
    // under macOS settings are stored on change rather than a separate apply or cancel stage
    //
    // https://developer.apple.com/design/human-interface-guidelines/macos/app-architecture/preferences/

    return true;
}

void Nedrysoft::UserSettingsPage::acceptSettings() {
}

QSize Nedrysoft::UserSettingsPage::sizeHint() const {
    return m_size;
}
