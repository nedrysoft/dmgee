/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 22/11/2020.
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

#include "GridGraphicsScene.h"

#include <QPainter>
#include <QSize>

Nedrysoft::GridGraphicsScene::GridGraphicsScene() {
    m_gridBrush = QBrush(Qt::transparent);
}

void Nedrysoft::GridGraphicsScene::drawForeground(QPainter *painter, const QRectF &rect) {
    painter->fillRect(rect, m_gridBrush);
}

void Nedrysoft::GridGraphicsScene::setGrid(QSize gridSize) {
    if (!gridSize.isValid()) {
        m_gridBrush = QBrush(Qt::transparent);
    } else {
        auto gridPixmap = QPixmap(gridSize.width(), gridSize.height());

        gridPixmap.fill(Qt::transparent);

        QPainter pixmapPainter(&gridPixmap);

        pixmapPainter.setPen(Qt::black);
        pixmapPainter.drawRect(gridPixmap.rect());

        pixmapPainter.end();

        m_gridBrush = QBrush(gridPixmap);
    }
}