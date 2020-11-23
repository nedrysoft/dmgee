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

#include "Image.h"

#include "Builder.h"
#include "GridGraphicsScene.h"

#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <memory>
#include <QPixmap>
#include <QWidget>

namespace Nedrysoft {
    /**
     * @brief       The PreviewWidget class draws a preview of the DMG, it includes overlays such as the actual icons
     *              of the application or shortcut and also detected feature locations and grid.
     */
    class PreviewWidget :
        public QWidget {
            private:
                Q_OBJECT

            public:
                /**
                 * @brief       Types of user defined QGraphicItems
                 */
                enum IconType {
                    Background,                         /**< User defined QGraphicsItem is a background. */
                    Centroid,                           /**< User defined QGraphicsItem is a centroid. */
                    Icon,                               /**< User defined QGraphicsItem is a icon. */
                    Shortcut                            /**< User defined QGraphicsItem is a shortcut. */
                };

                Q_ENUM(IconType)

            public:
                /**
                 * @brief       Constructs a new PreviewWidget instance which is a child of the parent.
                 *
                 * @param[in]   parent the owner widget.
                 */
                explicit PreviewWidget(QWidget *parent = nullptr);

                /**
                 * @brief       Sets the builder object for the preview.
                 *
                 * @note        The builder object contains the current configuration.
                 *
                 * @param[in]   builder the Builder configuration
                 */
                void setBuilder(Nedrysoft::Builder *builder);

                /**
                 * @brief       Sets the background pixmap to be displayed.
                 *
                 * @param[in]   pixmap the background image pixmap.
                 */
                void setPixmap(QPixmap &pixmap);

                /**
                 * @brief       Sets the snapping centroid locations.
                 *
                 * @param[in]   centroids the list of snap points.
                 */
                void setCentroids(QList<QPointF> &centroids);

                /**
                 * @brief       Clears the list of centroids.
                 */
                void clearCentroids();

                /**
                 * @brief       Sets whether the icons for the application/applications shortcut are shown.
                 *
                 * @param[in]   isVisible true if visible; otherwise false.
                 */
                void setIconsVisible(bool isVisible);

                /**
                 * @brief       Sets the size of the icons as they should appear in the DMG.
                 *
                 * @param[in]   size the width/height in pixels of the icon (square).
                 */
                void setIconSize(int size);

                /**
                 * @brief       Sets the size of the text as ot should appear in the DMG.
                 *
                 * @param[in]   size the size of the text in points.
                 */
                void setTextSize(int textSize);

                /**
                 * @brief       Sets the grid size.
                 *
                 * @param[in]   size the size of the text in points.
                 */
                void setGridSize(QSize gridSize);

                /**
                 * @brief       Scales the graphics view so that the scene fits inside the frame rectangle
                 */
                void fitToView();

                /**
                 * @brief       Adds a text item to the preview.
                 *
                 * @param[in]   pos the position of the text
                 * @param[in]   text the string to be displayed
                 */
                void addText(QPoint pos, QString text);

            protected:
                /**
                 * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
                 *
                 * @param[in]   event the event information.
                 */
                void paintEvent(QPaintEvent *event) override;

                /**
                 * @brief       Reimplements: QWidget::showEvent(QShowEvent *event).
                 *
                 * @param[in]   event the event information.
                 */
                void showEvent(QShowEvent *event) override;

                /**
                 * @brief       Adds an icon to the DMG.
                 *
                 * @param[in]   text is the text to be displayed with the icon.
                 * @param[in]   image the image to be displayed.
                 * @param[in]   point the initial location of the icon.
                 * @param[in]   iconType the type of icon being inserted.
                 * @param[in]   updateFunction the function to be called when the icon is moved.
                 */
                void addIcon(QString text, Nedrysoft::Image *image, const QPoint &point, IconType iconType, std::function<void(QPoint &point)> updateFunction);

                /**
                 * @brief       Reimplements: QWidget::resizeEvent(QResizeEvent *event).
                 *
                 * @param[in]   event the event information.
                 */
                void resizeEvent(QResizeEvent *event) override;

            private:
                QPixmap m_pixmap;                           //! the background image pixmap
                QPixmap m_targetPixmap;                     //! target snap location image
                QList<QPointF> m_centroids;                 //! centroid points

                QPointF m_iconPosition;                     //! holds position of icon for drag & drop

                GridGraphicsScene m_graphicsScene;          //! the graphics scene
                QGraphicsView m_graphicsView;               //! the rendered graphics scene
                QGraphicsItemGroup m_grid;                  //! the grid group

                QGridLayout m_layout;                       //! the layout to hold the widgets

                Nedrysoft::Builder *m_builder;              //! the builder object that contains the current configuration.
    };
}

#endif //NEDRYSOFT_PREVIEWWIDGET_H
