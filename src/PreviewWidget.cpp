/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 07/11/2020.
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

#include "PreviewWidget.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>

Nedrysoft::PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent),
    m_gridSize(20,20),
    m_gridIsVisible(true),
    m_gridShouldSNap(false) {

    m_targetPixmap = QPixmap(":/assets/target.png");
}

void Nedrysoft::PreviewWidget::setPixmap(QPixmap pixmap) {
    m_pixmap = pixmap;

    update();
}

void Nedrysoft::PreviewWidget::setCentroids(QList<QPointF> centroids) {
    m_centroids = centroids;

    update();
}

void Nedrysoft::PreviewWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    float width = m_pixmap.width(), height = m_pixmap.height();
    float scale = 1;
    float targetSize = 80;

    if ( (m_pixmap.width()>event->rect().width()) ||
         (m_pixmap.height()>event->rect().height()) ) {
        scale = qMin(static_cast<float>(event->rect().width())/width, static_cast<float>(event->rect().height())/height);

        width *= scale;
        height *= scale;
    }

    auto x = (static_cast<float>(event->rect().width()-width))*0.5;
    auto y = (static_cast<float>(event->rect().height()-height))*0.5;

    painter.drawPixmap(x, y, width, height, m_pixmap);

    targetSize = 80*scale;

    for(auto centroid : m_centroids) {
        painter.drawPixmap(x+(centroid.x()*scale)-(targetSize/2), y+(centroid.y()*scale)-(targetSize/2), targetSize, targetSize, m_targetPixmap);
    }

    if (m_gridIsVisible) {
        QPainterPath gridPath;

        for (auto i = 0; i < width / m_gridSize.width(); i++) {
            float gridX = ( i * m_gridSize.width()) + x;

            gridPath.moveTo(QPointF(gridX, y));
            gridPath.lineTo(QPointF(gridX, y + height));
        }

        for (auto i = 0; i < height / m_gridSize.height(); i++) {
            float gridY = ( i * m_gridSize.height()) + y;

            gridPath.moveTo(QPointF(x, gridY));
            gridPath.lineTo(QPointF(x + width, gridY));
        }

        painter.setPen(QPen(QColor(255, 255, 255, 45)));

        painter.drawPath(gridPath);
    }
}

void Nedrysoft::PreviewWidget::setGrid(QSize size, bool visible, bool snap) {
    m_gridSize = size;
    m_gridIsVisible = visible;
    m_gridShouldSNap = snap;

    update();
}
