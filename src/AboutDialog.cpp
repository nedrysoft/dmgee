/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 30/10/2020.
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

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QDesktopWidget>
#include <QDialog>
#include <QFont>
#include <QFontDatabase>
#include <QPainter>
#include <QString>

#include <QDebug>

constexpr auto splashScreenFilename = ":/images/splashscreen@2x.png";
constexpr auto fontFamily = "Poppins";
constexpr auto fontSize = 20;
constexpr auto aboutScreenWidth = 700;
constexpr auto textColour = qRgba(0xFF, 0xFF, 0xFF, 0xB0);
constexpr auto versionPosition = QPoint(45, 123);
constexpr auto bottomMargin = 28;

Nedrysoft::AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent, Qt::FramelessWindowHint) {

    m_backgroundPixmap = QPixmap(splashScreenFilename);

    m_backgroundPixmap = m_backgroundPixmap.scaledToWidth(aboutScreenWidth*m_backgroundPixmap.devicePixelRatio(), Qt::SmoothTransformation);

    resize((m_backgroundPixmap.size()/m_backgroundPixmap.devicePixelRatio()));

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_AlwaysStackOnTop);
}

void Nedrysoft::AboutDialog::focusOutEvent(QFocusEvent *event) {
    Q_UNUSED(event);

    close();
}

bool Nedrysoft::AboutDialog::event(QEvent *event) {
    switch(event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::KeyPress: {
            close();
            break;
        }

        default: {
            break;
        }
    }

    return QDialog::event(event);
}

void Nedrysoft::AboutDialog::paintEvent(QPaintEvent *paintEvent) {
    auto font = QFont(fontFamily, fontSize, QFont::Weight::Bold);
    auto versionText = QString("%1.%2.%3").arg(APPLICATION_GIT_YEAR).arg(APPLICATION_GIT_MONTH).arg(APPLICATION_GIT_DAY);
    auto gitDetails = QString("%1 (%2)").arg(APPLICATION_GIT_BRANCH).arg(APPLICATION_GIT_HASH);

    QPainter painter(this);

    painter.drawPixmap(0,0, m_backgroundPixmap);

    painter.save();

    painter.setFont(font);

    painter.setPen(QColor::fromRgba(textColour));

    auto textRect = QRect(versionPosition, rect().bottomRight()-versionPosition);

    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop, versionText);

    painter.setPen(Qt::white);

    font = QFont(fontFamily, fontSize, QFont::Weight::Light);

    painter.setFont(font);

    painter.drawText(QPoint(textRect.left(), rect().bottom()-bottomMargin), gitDetails);

    painter.restore();
}
