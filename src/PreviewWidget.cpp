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

#include "Image.h"
#include "SnappedGraphicsPixmapItem.h"

#include <QDebug>
#include <QDrag>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QMimeData>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

Nedrysoft::PreviewWidget::PreviewWidget(QWidget *parent) :
        QWidget(parent),
        m_gridSize(20,20),
        m_iconSize(160),
        m_gridIsVisible(true),
        m_gridShouldSNap(false),
        m_snapToFeatures(true),
        m_iconPosition() {

    m_targetPixmap = QPixmap(":/icons/target.png");

    m_graphicsView.setScene(&m_graphicsScene);

    m_graphicsView.setInteractive(true);

    m_layout.setMargin(0);

    m_layout.addWidget(&m_graphicsView);

    setLayout(&m_layout);

/*
    for (auto i = 0; i < width / m_gridSize.width(); i++) {
        float gridX = ( i * m_gridSize.width()) + x;

        auto line = new QGraphicsLineItem(gridX, y, gridX, y + height);

        line.setPen(QPen(QColor(255, 255, 255, 45)));

        m_grid.addToGroup(line);
    }

    for (auto i = 0; i < height / m_gridSize.height(); i++) {
        float gridY = ( i * m_gridSize.height()) + y;

        gridPath.moveTo(QPointF(x, gridY));
        gridPath.lineTo(QPointF(x + width, gridY));
    }

    painter.setPen();

    painter.drawPath(gridPath);
*/
}

void Nedrysoft::PreviewWidget::setPixmap(QPixmap &pixmap) {
    m_pixmap = pixmap;

    for (auto item : m_graphicsScene.items()) {
        if (item->data(Qt::UserRole).isValid()) {
            switch(item->data(Qt::UserRole).value<int>()) {
                case Background:
                case Centroid: {
                    m_graphicsScene.removeItem(item);
                    break;
                }
            }
        }
    }

    auto item = m_graphicsScene.addPixmap(m_pixmap);

    item->setData(Qt::UserRole, Background);
    item->setZValue(0);

    setCentroids(m_centroids);
}

void Nedrysoft::PreviewWidget::setCentroids(QList<QPointF> &centroids) {
    m_centroids = centroids;

    for (auto item : m_graphicsScene.items()) {
        if ((item->data(Qt::UserRole).isValid()) && (item->data(Qt::UserRole)==Centroid)) {
            m_graphicsScene.removeItem(item);
        }
    }

    for (auto const centroid : m_centroids) {
        auto targetItem = new QGraphicsPixmapItem(m_targetPixmap);

        targetItem->setPos(centroid.x()-(static_cast<float>(m_targetPixmap.width()) / 2.0), centroid.y()-(static_cast<float>(m_targetPixmap.height()) / 2.0));
        targetItem->setData(Qt::UserRole, Centroid);
        targetItem->setZValue(0.5);

        m_graphicsScene.addItem(targetItem);
    }
}

void Nedrysoft::PreviewWidget::setGrid(QSize size, bool visible, bool snap) {
    m_gridSize = size;
    m_gridIsVisible = visible;
    m_gridShouldSNap = snap;
}

void Nedrysoft::PreviewWidget::addIcon(Nedrysoft::Image *image, const QPoint &point, IconType iconType, std::function<void(QPoint &point)> updateFunction) {
    auto pixmap = QPixmap::fromImage(image->image());

    auto snappedIcon = new SnappedGraphicsPixmapItem([pixmap, this, updateFunction](const QPoint &point) {
        QPoint snapPoint = point;
        float scale = static_cast<float>(m_iconSize)/static_cast<float>(pixmap.width());
        float width = static_cast<float>(pixmap.width())*scale;
        float height = static_cast<float>(pixmap.height())*scale;

        if (m_snapToFeatures) {
            float closestDistance = MAXFLOAT;

            for (auto const centroid : m_centroids) {
                auto dx = centroid.x()-point.x();
                auto dy = centroid.y()-point.y();

                float distance = sqrtf(powf(dx, 2) + powf(dy, 2));

                if ((distance<50) && (distance < closestDistance)) {
                    snapPoint = centroid.toPoint();

                    closestDistance = distance;
                }
            }
        }

        updateFunction(snapPoint);

        return snapPoint;
    });

    snappedIcon->setPixmap(pixmap);
    snappedIcon->setPos(point);
    snappedIcon->setOffset(-(static_cast<float>(pixmap.width())/2.0), -(static_cast<float>(pixmap.height())/2.0));
    snappedIcon->setScale(static_cast<float>(m_iconSize)/static_cast<float>(pixmap.width()));
    snappedIcon->setData(Qt::UserRole, iconType);
    snappedIcon->setZValue(1);
    snappedIcon->setTransformationMode(Qt::SmoothTransformation);

    m_graphicsScene.addItem(snappedIcon);
}

void Nedrysoft::PreviewWidget::setIconsVisible(bool isVisible) {
    for (auto item : m_graphicsScene.items()) {
        if (item->data(Qt::UserRole).isValid()) {
            switch(item->data(Qt::UserRole).toInt()) {
                case Icon:
                case Shortcut: {
                    item->setVisible(isVisible);
                    break;
                }
            }
        }
    }
}

void Nedrysoft::PreviewWidget::setIconSize(int size) {
    m_iconSize = size;

    for (auto item : m_graphicsScene.items()) {
        if (item->data(Qt::UserRole).isValid()) {
            switch ( item->data(Qt::UserRole).toInt()) {
                case Icon:
                case Shortcut: {
                    auto pixmapItem = dynamic_cast<SnappedGraphicsPixmapItem *>(item);

                    pixmapItem->setScale(static_cast<float>(m_iconSize)/static_cast<float>(pixmapItem->pixmap().width()));
                    break;
                }
            }
        }
    }
}

void Nedrysoft::PreviewWidget::clearCentroids() {
    m_centroids.clear();

    for (auto item : m_graphicsScene.items()) {
        if ((item->data(Qt::UserRole).isValid()) && (item->data(Qt::UserRole)==Centroid)) {
            m_graphicsScene.removeItem(item);
        }
    }
}

void Nedrysoft::PreviewWidget::setTextSize(int textSize) {
    //! TODO:
}

void Nedrysoft::PreviewWidget::fitToView() {
    m_graphicsView.fitInView(m_graphicsScene.sceneRect(), Qt::KeepAspectRatio);
}

void Nedrysoft::PreviewWidget::addText(QPoint pos, QString text) {
    constexpr auto fontFamily = "Open Sans";
    constexpr auto fontSize = 14;
    auto font = QFont(fontFamily, fontSize, QFont::Weight::Normal);

    auto textItem = m_graphicsScene.addText(text);

    textItem->setPos(pos);
    textItem->setFont(font);
    textItem->setDefaultTextColor(Qt::white);

    textItem->setZValue(1);

    textItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    textItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void Nedrysoft::PreviewWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    /*QPainter painter(this);

    painter.fillRect(event->rect(), Qt::lightGray);*/
}