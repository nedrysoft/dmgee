/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 20/11/2020.
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

#include "AnsiEscape.h"

constexpr auto ESC = "\x1b";

QString Nedrysoft::AnsiEscape::fore(Nedrysoft::AnsiColour colour) {
    int escapeCode = static_cast<int>(colour);

    return QString("%1[%2m").arg(ESC).arg(escapeCode);
}

QString Nedrysoft::AnsiEscape::back(Nedrysoft::AnsiColour colour) {
    int escapeCode = static_cast<int>(colour)+10;

    return QString("%1[%2m").arg(ESC).arg(escapeCode);
}

QString Nedrysoft::AnsiEscape::style(Nedrysoft::AnsiStyle style) {
    int escapeCode = static_cast<int>(style);

    return QString("%1[%2m").arg(ESC).arg(escapeCode);
}

QString Nedrysoft::AnsiEscape::reset() {
    int escapeCode = static_cast<int>(RESET_ALL);

    return QString("%1[%2m").arg(ESC).arg(escapeCode);
}

QString Nedrysoft::AnsiEscape::fore(int r, int g, int b) {
    return QString("%1[38;2;%2;%3;%4m").arg(ESC).arg(r).arg(g).arg(b);
}

QString Nedrysoft::AnsiEscape::back(int r, int g, int b) {
    return QString("%1[48;2;%2;%3;%4m").arg(ESC).arg(r).arg(g).arg(b);
}

QString Nedrysoft::AnsiEscape::fore(QColor colour) {
    return QString("%1[38;2;%2;%3;%4m").arg(ESC).arg(colour.red()).arg(colour.green()).arg(colour.blue());
}

QString Nedrysoft::AnsiEscape::back(QColor colour) {
    return QString("%1[48;2;%2;%3;%4m").arg(ESC).arg(colour.red()).arg(colour.green()).arg(colour.blue());
}

QString Nedrysoft::AnsiEscape::link(QString url, QString description) {
    return QString("%1]8;;%2%1\\%3%1]8;;%1\\").arg(ESC).arg(url).arg(description);
}

QString Nedrysoft::AnsiEscape::underline(bool state) {
    if (state) {
        return QString("%1[1m").arg(ESC);
    } else {
        return QString("%1[0m").arg(ESC);
    }
}
