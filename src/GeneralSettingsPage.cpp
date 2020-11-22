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

#include "GeneralSettingsPage.h"

#include "MainWindow.h"
#include "ui_GeneralSettingsPage.h"

#include <QDebug>
#include <QMessageBox>

Nedrysoft::GeneralSettingsPage::GeneralSettingsPage(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::GeneralSettingsPage) {

    ui->setupUi(this);

#if defined(Q_OS_MACOS)
    m_size = QSize(qMax(minimumSizeHint().width(), size().width()), qMax(minimumSizeHint().height(), size().height()));
#else
    m_size = minimumSizeHint();
#endif

}

Nedrysoft::GeneralSettingsPage::~GeneralSettingsPage()
{
    delete ui;
}

bool Nedrysoft::GeneralSettingsPage::canAcceptSettings()
{
    return true;
}

void Nedrysoft::GeneralSettingsPage::acceptSettings()
{
}

QSize Nedrysoft::GeneralSettingsPage::sizeHint() const
{
    return m_size;
}
