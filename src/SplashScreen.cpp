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

#include "SplashScreen.h"

#include <QPainter>

constexpr auto splashScreenFilename = ":/images/splashscreen.png";
constexpr auto fontFamily = "Poppins";
constexpr auto fontSize = 20;
constexpr auto splashScreenWidth = 700;
constexpr auto textColour = qRgba(0xFF, 0xFF, 0xFF, 0xB0);
constexpr auto versionPosition = QPoint(45, 123);

Nedrysoft::SplashScreen::SplashScreen() :
        QSplashScreen(QPixmap(), Qt::WindowStaysOnTopHint) {

    auto pixmap = QPixmap(splashScreenFilename);

    setPixmap(pixmap.scaledToWidth(splashScreenWidth*pixmap.devicePixelRatio(), Qt::SmoothTransformation));

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    show();
}

Nedrysoft::SplashScreen::~SplashScreen() {
    delete getInstance();
}

Nedrysoft::SplashScreen *Nedrysoft::SplashScreen::getInstance() {
    static Nedrysoft::SplashScreen *instance;

    if (!instance) {
        instance = new Nedrysoft::SplashScreen;
    }

    return(instance);
}

void Nedrysoft::SplashScreen::drawContents(QPainter *painter) {
    auto font = QFont(fontFamily, fontSize, QFont::Weight::Bold);
    auto versionText = QString("%1.%2.%3").arg(APPLICATION_GIT_YEAR).arg(APPLICATION_GIT_MONTH).arg(APPLICATION_GIT_DAY);

    painter->save();

    painter->setFont(font);

    painter->setPen(QColor::fromRgba(textColour));

    auto textRect = QRect(versionPosition, rect().bottomRight()-versionPosition);

    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop, versionText);

    painter->restore();
}
