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

#ifndef NEDRYSOFT_GRIDGRAPHICSSCENE_H
#define NEDRYSOFT_GRIDGRAPHICSSCENE_H

#include <QGraphicsScene>

namespace Nedrysoft {
    /**
     * @brief       The GridGraphicsScene class provides a grid overlay.
     */
    class GridGraphicsScene :
            public QGraphicsScene {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new GridGraphicsScene.
             */
            GridGraphicsScene();

            /**
             * @brief       Sets the grid size.
             *
             * @note        An invalid size will result in no grid being drawn.
             *
             * @params[in]  gridSize the size of grid.
             */
            void setGrid(QSize gridSize);

        protected:
            /**
             * @brief       Reimplements: QWidget::drawForeground(QPainter *painter, const QRectF &rect).
             *
             * @param[in]   painter the painter object to draw to.
             * @param[in]   event the event information.
             */
            void drawForeground(QPainter *painter, const QRectF &rect) override;

        private:
            QBrush m_gridBrush;                         //! A brush that has a single grid cell.

    };
};

#endif //NEDRYSOFT_GRIDGRAPHICSSCENE_H
