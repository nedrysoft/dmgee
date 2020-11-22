/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 08/11/2020.
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

#include "SnappedGraphicsPixmapItem.h"

#include <QGraphicsScene>
#include <QMenu>

Nedrysoft::SnappedGraphicsPixmapItem::SnappedGraphicsPixmapItem(std::function<QPoint(const QPoint &point)> snapFunction) :
        m_snapFunction(snapFunction) {

    setFlag(ItemIsMovable, true);
    setFlag(ItemSendsGeometryChanges, true);
}

Nedrysoft::SnappedGraphicsPixmapItem::SnappedGraphicsPixmapItem() :
        m_snapFunction([](const QPoint &point) {
            return point;
        }) {
}

QVariant Nedrysoft::SnappedGraphicsPixmapItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    if (change==QGraphicsItem::ItemPositionChange) {
        return m_snapFunction(value.toPoint());;
    }

    return QGraphicsPixmapItem::itemChange(change, value);
}

void Nedrysoft::SnappedGraphicsPixmapItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu popupMenu;

    auto deleteAction = popupMenu.addAction(QObject::tr("Delete"));
    auto replaceAction = popupMenu.addAction(QObject::tr("Replace..."));

    auto selectedAction = popupMenu.exec(event->screenPos());

    if (selectedAction==deleteAction) {
        this->scene()->removeItem(this);
    } else if (selectedAction==replaceAction) {
        // TODO: replace image!
    }
}

int Nedrysoft::SnappedGraphicsPixmapItem::type() const {
    return UserType+1;
}