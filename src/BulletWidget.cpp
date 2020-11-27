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

#include "BulletWidget.h"

#include <QFontMetrics>
#include <QPaintEvent>
#include <QPainter>

constexpr auto imageDimensions = 14;
constexpr auto bulletWidth = 24;
constexpr auto textYAdjustment = -1;

Nedrysoft::BulletWidget::BulletWidget(QWidget *parent) :
        QWidget(parent) {

    m_font = font();

    QFontMetrics fontMetrics(m_font);

    m_fontHeight = fontMetrics.boundingRect("X").height();

    m_fontDescent = fontMetrics.descent();

    setMinimumHeight(m_fontHeight);
    setMaximumHeight(m_fontHeight);
}

Nedrysoft::BulletWidget::BulletWidget(QString text, QPixmap pixmap, QString toolTip, QWidget *parent) :
        QWidget(parent),
        m_text(text),
        m_pixmap(pixmap),
        m_toolTip(toolTip) {

    auto tempFont = font();

    QFontMetrics fontMetrics(tempFont);

    m_fontHeight = fontMetrics.boundingRect("X").height();

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

void Nedrysoft::BulletWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    QRect widgetRect(rect());

    painter.drawPixmap(0, 0, imageDimensions, imageDimensions, m_pixmap);

    widgetRect.setLeft(bulletWidth);
    widgetRect.setTop(textYAdjustment);

    painter.drawText(widgetRect, Qt::AlignTop, m_text);
}

QSize Nedrysoft::BulletWidget::sizeHint() const {
    return QSize(100,m_fontHeight);
}