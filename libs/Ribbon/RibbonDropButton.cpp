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

#include "RibbonDropButton.h"
#include <QApplication>
#include <QDebug>
#include <QSpacerItem>
#include "ThemeSupport.h"

Nedrysoft::Ribbon::RibbonDropButton::RibbonDropButton(QWidget *parent) :
        QWidget(parent),
        m_iconSize(QSize(RibbonDropButtonDefaultIconWidth,RibbonDropButtonDefaultIconHeight)) {

    m_layout = new QVBoxLayout;
    m_mainButton = new QPushButton;
    m_dropButton = new QPushButton;

    m_layout->addWidget(m_mainButton);
    m_layout->addWidget(m_dropButton);

    m_layout->setContentsMargins(0,0,0,0);

    m_dropButton->setMinimumHeight(RibbonDropButtonDefaultHeight);
    m_dropButton->setMaximumHeight(RibbonDropButtonDefaultHeight);

    m_dropButton->setIconSize(QSize(5,4));

    m_dropButton->setFlat(true);
    m_mainButton->setFlat(true);

    setLayout(m_layout);

    connect(qobject_cast<QApplication *>(QCoreApplication::instance()), &QApplication::paletteChanged, [=] (const QPalette &) {
        updateDropIcon();
    });

    connect(m_mainButton, &QPushButton::clicked, [=] (bool checked) {
        Q_EMIT clicked(false);
    });

    connect(m_dropButton, &QPushButton::clicked, [=] (bool checked) {
        Q_EMIT clicked(true);
    });

    updateDropIcon();
    updateSizes();
}

Nedrysoft::Ribbon::RibbonDropButton::~RibbonDropButton() {
    m_mainButton->deleteLater();
    m_dropButton->deleteLater();
    m_layout->deleteLater();
}

void Nedrysoft::Ribbon::RibbonDropButton::updateDropIcon() {
    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        m_dropButton->setIcon(QIcon(":/Nedrysoft/Ribbon/icons/arrow_drop-light@2x.png"));
    } else {
        m_dropButton->setIcon(QIcon(":/Nedrysoft/Ribbon/icons/arrow_drop-dark@2xpng"));
    }
}

QIcon Nedrysoft::Ribbon::RibbonDropButton::icon() {
    return m_mainButton->icon();
}

void Nedrysoft::Ribbon::RibbonDropButton::setIcon(QIcon &icon) {
    m_mainButton->setIcon(icon);
}

QSize Nedrysoft::Ribbon::RibbonDropButton::iconSize() {
    return m_iconSize;
}

void Nedrysoft::Ribbon::RibbonDropButton::setIconSize(QSize iconSize)
{
    m_iconSize = iconSize;

    updateSizes();
}

void Nedrysoft::Ribbon::RibbonDropButton::updateSizes() {
    m_mainButton->setMinimumSize(m_iconSize);
    m_mainButton->setMaximumSize(QSize(16777215, m_iconSize.height()));
    m_mainButton->setIconSize(m_iconSize);
}