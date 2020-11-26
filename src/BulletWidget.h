/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 26/11/2020.
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

#ifndef NEDRYSOFT_BULLETWIDGET_H
#define NEDRYSOFT_BULLETWIDGET_H

#include <QFont>
#include <QPaintEvent>
#include <QPixmap>
#include <QWidget>

namespace Nedrysoft {
    /**
     * @brief       The BulletWidget class provides a widget with a bullet to the left of
     *              user defined text.
     */
    class BulletWidget :
        public QWidget {
            private:
                Q_OBJECT

            public:
                /**
                 * @brief       Constructs a new SettingsDialog instance which is a child of the parent.
                 *
                 * @param[in]   parent is the the owner of the child.
                 */
                BulletWidget(QWidget *parent = nullptr);

                /**
                 * @brief       Constructs a new SettingsDialog instance which is a child of the parent.
                 *
                 * @param[in]   text the text that is to appear next to the bullet.
                 * @param[in]   pixmap the pixmap bullet to use.
                 * @param[in]   toolTip the tooltip text that is to appear when hovered.
                 * @param[in]   parent is the the owner of the child.
                 */
                BulletWidget(QString text, QPixmap pixmap, QString toolTip = QString(), QWidget *parent = nullptr);

            protected:
                /**
                 * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
                 *
                 * @param[in]   event the event information.
                 */
                void paintEvent(QPaintEvent *event) override;

            private:
                QString m_text;                             //! the text to appear to the right of the bullet.
                QPixmap m_pixmap;                           //! the pixmap to use for the bullet
                QString m_toolTip;                          //! the tooltip text to display on hover
                int m_fontHeight;                           //! the calculated font height
                int m_fontDescent;                          //! the dalculated descent of the font
                QFont m_font;                               //! the font to use for rendering.
     };
};


#endif //NEDRYSOFT_BULLETWIDGET_H
