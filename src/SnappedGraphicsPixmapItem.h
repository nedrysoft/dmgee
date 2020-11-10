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

#ifndef NEDRYSOFT_SNAPPEDGRAPHICSPIXMAPITEM_H
#define NEDRYSOFT_SNAPPEDGRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QPoint>
#include <functional>

namespace Nedrysoft {
    /**
     * @brief       A pixmap graphics item that provides a function to allow a custom snapping function to be used
     *              to snap the centre point to a point of interest (centroid of feature or grid point)
     */
    class SnappedGraphicsPixmapItem :
            public QGraphicsPixmapItem {
        public:
            /**
             * @brief       Constructs a graphics item with the given snapping function
             *
             * @param[in]   snapFunction is the function to be used to snap the centre point to.
             */
            SnappedGraphicsPixmapItem(std::function<QPoint (const QPoint &point)> snapFunction);

            /**
             * @brief       Constructs a graphics item without a snapping function
             */
            SnappedGraphicsPixmapItem();

        protected:
            /**
             * @brief      Overloaded function called when the item is changed, currently used to track positional
             *             changes of the object to allow the supplied snapping function to be used.
             *
             * @param[in]  change is the type of item change
             * @param[in]  value is the value relating to the item change event
             *
             * @returns    The modified value if changed; otherwise a copy of the original value
             */
            virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

            /**
             * @brief       Called when the context menu is requested, displays a popup menu to allow the user to
             *              perform an action
             *
             * @param[in]   event contains information about the context event.
             */
            virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

        private:
            std::function<QPoint(const QPoint &)> m_snapFunction;               //! the callback function for snapping
    };
}

#endif //NEDRYSOFT_SNAPPEDGRAPHICSPIXMAPITEM_H
