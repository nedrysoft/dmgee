/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 18/11/2020.
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

#include "HTermApi.h"
#include <QDebug>

void Nedrysoft::HTermApi::onTerminalReady() {
    Q_EMIT terminalReady();
}

void Nedrysoft::HTermApi::onContextMenu() {
    Q_EMIT contextMenu();
}

void Nedrysoft::HTermApi::print(QString string) {
    Q_EMIT doPrint(string);
}

void Nedrysoft::HTermApi::println(QString string) {
    Q_EMIT doPrintln(string);
}

void Nedrysoft::HTermApi::urlClicked(QString url) {
    Q_EMIT openUrl(url);
}

void Nedrysoft::HTermApi::clear() {
    Q_EMIT doClear();
}

void Nedrysoft::HTermApi::onTerminalBuffer(QString terminalBuffer) {
    Q_EMIT Nedrysoft::HTermApi::terminalBuffer(terminalBuffer);
}

void Nedrysoft::HTermApi::getTerminalBuffer() {
    Q_EMIT doGetTerminalBuffer();
}
