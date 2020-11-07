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

#ifndef NEDRYSOFT_PREVIEWWIDGET_H
#define NEDRYSOFT_PREVIEWWIDGET_H

#include <QPixmap>
#include <QWidget>

namespace Nedrysoft {
    /**
     * @brief       Widget that draws a preview of the DMG (including snap targets)
     */
    class PreviewWidget :
        public QWidget {
            private:
                Q_OBJECT

            public:
                /**
                 * @brief           Constructs a preview widget.
                 *
                 * @param[in]       parent is the the owner of the dialog.
                 */
                PreviewWidget(QWidget *parent = nullptr);

                /**
                 * @brief           Sets the background pixmap to be used
                 *
                 * @param[in]       pixmap is the pixmap to be used as the background image
                 */
                void setPixmap(QPixmap pixmap);

                /**
                 * @brief           Sets the snapping centroid locations
                 *
                 * @param[in]       centroids list of snap points
                 */
                void setCentroids(QList<QPointF> centroids);

                /**
                 * @brief           Sets the grid for the widget
                 *
                 * @param[in]       size is the pitch of the grid
                 * @param[in]       visible true if grid shown; otherwise false.
                 * @paramin]        snap true if cursor should snap to grid; otherwise false.
                 */
                void setGrid(QSize size, bool visible, bool snap);

            protected:
                /**
                 * @brief           Handles the repainting of the image
                 * @param[in]       event contains the details about the paint event request
                 */
                void paintEvent(QPaintEvent *event) override;

            private:
                QPixmap m_pixmap;                           //! the background image pixmap
                QPixmap m_targetPixmap;                     //! target snap location image
                QList<QPointF> m_centroids;                 //! centroid points

                QSize m_gridSize;                           //! grid size
                bool m_gridIsVisible;                       //! whether the grid is drawn
                bool m_gridShouldSNap;                      //! whether the cursor should snap to the grid
    };
}

#endif //NEDRYSOFT_PREVIEWWIDGET_H
