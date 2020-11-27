/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 25/11/2020.
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

#ifndef NEDRYSOFT_THEMEDOUTLINEVIEW_H
#define NEDRYSOFT_THEMEDOUTLINEVIEW_H

#include <QWidget>

#include <QTreeView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLinearGradient>
#include <QPainter>
#include <QPushButton>
#include <QStandardItemModel>

namespace Nedrysoft {
    /**
     * @brief       the ThemedOutlineButtonBox widget mimics a flat design button box consistent with macOS.
     */
    class ThemedOutlineButtonBox :
            public QWidget {

        private:
            Q_OBJECT;

        public:
            /**
             * @brief       Constructs a new ThemedOutlineButtonBox instance.
             */
            ThemedOutlineButtonBox(QWidget * parent = nullptr);

            /**
             * @brief       This signal is emitted when a button has been clicked
             *
             * @param[in]   buttonIndex the index of the button that was blicked.
             */
            Q_SIGNAL void buttonClicked(int buttonIndex);

        private:
            /**
             * @brief       Renders the widget content to a pixmap.
             *
             * @details     This widget draws to a pixmap to ensure that it draws without antialiasing, drawing
             *              directly in the paintEvent to the widget on macOS results in antialiasing which doesn't
             *              result in pixel perfect rendering.
             *
             * @param[in]   rect the rectangle of the widget.
             */
            void renderToPixmap(QRect rect);

            /**
             * @brief       Check if the button is highlighted.
             *
             * @param[in]   buttonIndex the index of the button to check.
             *
             * @returns     true if highlighted; otherwise false.
             */
            bool buttonIsHighlighted(int buttonIndex);

        protected:
            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @param[in]   event the event information.
             */
            void paintEvent(QPaintEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::mousePressEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            void mousePressEvent(QMouseEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::mouseReleaseEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            void mouseReleaseEvent(QMouseEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::mouseMoveEvent(QMouseEvent *event).
             *
             * @param[in]   event the event information.
             */
            void mouseMoveEvent(QMouseEvent *event) override;

            /**
             * @brief       Reimplements an access function for property: QWidget::sizeHint.
             *
             * @returns     the recommended size for the widget.
             */
            QSize sizeHint();

            /**
             * @brief       Reimplements: QWidget::resizeEvent(QResizeEvent *event).
             *
             * @param[in]   event the event information.
             */
            void resizeEvent(QResizeEvent *event) override;

        private:
            QPixmap m_pixmap;                                       //! the widget pixmap
            int m_buttonIndex;                                      //! the current button index that is "hit"
    };

    /**
     * @brief       the ThemedOutlineView widget provides a QTreeView with a button box at the bottom..
     */
    class ThemedOutlineView :
            public QWidget {

        private:
            Q_OBJECT;

        public:
            /**
             * @brief       Constructs a new ThemedOutlineView instance.
             */
            ThemedOutlineView(QWidget *parent=nullptr);

            /**
             * @brief       Sets the item model which the TreeView is bound to
             *
             * @param[in]   model the item model.
             */
            void setItemModel(QStandardItemModel *model);

        private:
            /**
             * @brief       Renders the widget content to a pixmap.
             *
             * @details     This widget draws to a pixmap to ensure that it draws without antialiasing, drawing
             *              directly in the paintEvent to the widget on macOS results in antialiasing which doesn't
             *              result in pixel perfect rendering.
             *
             * @param[in]   rect the rectangle of the widget.
             */
            void renderToPixmap(QRect rect);

        protected:
            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @param[in]   event the event information.
             */
            void paintEvent(QPaintEvent *event) override;

            /**
             * @brief       Reimplements: QWidget::resizeEvent(QResizeEvent *event).
             *
             * @param[in]   event the event information.
             */
            void resizeEvent(QResizeEvent *event) override;

        public:
            /**
             * @brief       This signal is emitted when TreeView selection changes.
             *
             * @index[in]   index the index of the current selection if valid.
             */
            Q_SIGNAL void clicked(const QModelIndex &index);

            /**
             * @brief       This signal is emitted when a button has been clicked
             *
             * @param[in]   buttonIndex the index of the button that was blicked.
             */
            Q_SIGNAL void buttonClicked(int buttonIndex);

        private:
            QTreeView *m_treeViewWidget;                                //! the treeview widget
            QVBoxLayout *m_layout;                                      //! main layout (QTreeView + ThemedOutlineButtonBox)
            ThemedOutlineButtonBox *m_buttonBox;                        //! the button box at the bottom of the widget
            QPixmap m_pixmap;                                           //! pixmap for rendering the widget content
    };
}

#endif //NEDRYSOFT_THEMEDOUTLINEVIEW_H
