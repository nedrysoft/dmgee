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
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

Nedrysoft::PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent)
{
}

void Nedrysoft::PreviewWidget::setPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;

    update();
}

void Nedrysoft::PreviewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    float width = m_pixmap.width(), height = m_pixmap.height();

    if ( (m_pixmap.width()>event->rect().width()) ||
         (m_pixmap.height()>event->rect().height()) ) {
        auto scale = qMin(static_cast<float>(event->rect().width())/width, static_cast<float>(event->rect().height())/height);

        width *= scale;
        height *= scale;
    }

    auto x = (static_cast<float>(event->rect().width()-width))*0.5;
    auto y = (static_cast<float>(event->rect().height()-height))*0.5;

    painter.drawPixmap(x, y, width, height, m_pixmap);
}