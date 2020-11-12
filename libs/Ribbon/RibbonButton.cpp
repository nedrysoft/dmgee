/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * https://github.com/fizzyade
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

#include "RibbonButton.h"
#include "RibbonFontManager.h"
#include <QApplication>
#include <QDebug>
#include <QSpacerItem>
#include "ThemeSupport.h"

Nedrysoft::Ribbon::RibbonButton::RibbonButton(QWidget *parent) :
        QWidget(parent),
        m_iconSize(QSize(RibbonButtonDefaultIconWidth,RibbonButtonDefaultIconHeight)) {

    m_layout = new QVBoxLayout;
    m_mainButton = new QPushButton;
    m_buttonLabel = new QLabel;

    m_buttonLabel->setAlignment(Qt::AlignHCenter);

    auto fontManager = RibbonFontManager::getInstance();

    auto font = QFont(fontManager->normalFont(), RibbonButtonDefaultFontSize);

    m_buttonLabel->setFont(font);

    m_layout->addWidget(m_mainButton);
    m_layout->addWidget(m_buttonLabel);

    m_layout->setAlignment(Qt::AlignHCenter);

    m_layout->setContentsMargins(0,0,0,0);

    m_mainButton->setFlat(true);

    setLayout(m_layout);

    connect(qobject_cast<QApplication *>(QCoreApplication::instance()), &QApplication::paletteChanged, [=] (const QPalette &) {
    });

    connect(m_mainButton, &QPushButton::clicked, [=] (bool checked) {
        emit clicked();
    });

    updateSizes();
}

Nedrysoft::Ribbon::RibbonButton::~RibbonButton() {
    m_mainButton->deleteLater();
    m_buttonLabel->deleteLater();
    m_layout->deleteLater();
}

QIcon Nedrysoft::Ribbon::RibbonButton::icon() {
    return m_mainButton->icon();
}

void Nedrysoft::Ribbon::RibbonButton::setIcon(QIcon &icon) {
    m_mainButton->setIcon(icon);
}

QSize Nedrysoft::Ribbon::RibbonButton::iconSize() {
    return m_iconSize;
}

void Nedrysoft::Ribbon::RibbonButton::setIconSize(QSize iconSize)
{
    m_iconSize = iconSize;

    updateSizes();
}

QString Nedrysoft::Ribbon::RibbonButton::text() {
    return m_buttonLabel->text();
}

void Nedrysoft::Ribbon::RibbonButton::setText(QString text) {
    m_buttonLabel->setText(text);
}

void Nedrysoft::Ribbon::RibbonButton::updateSizes() {
    m_mainButton->setMinimumSize(m_iconSize);
    m_mainButton->setMaximumSize(QSize(16777215, m_iconSize.height()));
    //m_mainButton->setMaximumSize(QSize(m_iconSize.height(), m_iconSize.height()));
    m_mainButton->setIconSize(m_iconSize);
}