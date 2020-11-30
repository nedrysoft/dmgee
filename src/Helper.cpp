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

#include "Helper.h"

#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>

QString Nedrysoft::Helper::resolvedPath(QString filename) {
    return QString(filename).replace(QRegularExpression("(^~)"), QDir::homePath());
}

QString Nedrysoft::Helper::normalizedPath(QString filename) {
    auto tempFilename = resolvedPath(filename);
    auto homePath = QDir::homePath();

    if (tempFilename.startsWith(homePath)) {
        filename = tempFilename.replace(QRegularExpression("^"+QRegularExpression::escape(homePath)),"~");
    }

    return filename;
}
