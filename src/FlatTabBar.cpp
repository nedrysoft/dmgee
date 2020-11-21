/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 19/11/2020.
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

#include "FlatTabBar.h"

#include <QApplication>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionTabV3>

void Nedrysoft::FlatTabBar::paintEvent(QPaintEvent *event) {
    QStylePainter painter(this);
    QStyleOptionTab opt;
    QColor backgroundColour = qApp->palette().color(QPalette::Window);
    QPen borderPen;

    borderPen.setWidth(4);
    borderPen.setColor(backgroundColour);

    painter.save();

    painter.fillRect(event->rect(), backgroundColour);

    for (int currentTabIndex = 0; currentTabIndex < count(); currentTabIndex++) {
        initStyleOption(&opt, currentTabIndex);

        painter.fillRect(opt.rect, Qt::black);

        auto borderRect = opt.rect.adjusted(0,0,0,-2);

        if (currentTabIndex==currentIndex()) {
            painter.save();

            painter.setPen(borderPen);

            if (!(opt.position==QStyleOptionTab::Beginning) || (opt.position==QStyleOptionTab::OnlyOneTab)) {
                painter.drawLine(QPoint(borderRect.bottomLeft()), borderRect.topLeft());
            }

            painter.drawLine(borderRect.topLeft(), borderRect.topRight());
            painter.drawLine(borderRect.topRight(), borderRect.bottomRight());

            painter.restore();
        }

        painter.drawControl(QStyle::CE_TabBarTabLabel, opt);
    }

    painter.restore();
}