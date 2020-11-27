/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 27/11/2020.
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

#include "ThemedOutlineViewButtonBox.h"

#include <QPainter>
#include <QMouseEvent>

constexpr auto buttonWidth = 30;
constexpr auto textColour = qRgb(0xFF, 0xFF, 0xFF);
constexpr auto buttonBoxHeight = 18;
constexpr auto buttonBoxWidth = 30;
constexpr auto borderWidth = 1;
constexpr auto borderColor = qRgb(0x21,0x21,0x21);

constexpr QRgb backgroundGradient[] = {
    qRgb(0x4b, 0x4b, 0x4b),
    qRgb(0x49, 0x49, 0x49),
    qRgb(0x46, 0x46, 0x46)
};

constexpr QRgb backgroundGradientHighlighted[] = {
    qRgb(0x60, 0x60, 0x60),
    qRgb(0x5f, 0x5f, 0x5f),
    qRgb(0x57, 0x57, 0x57)
};

Nedrysoft::ThemedOutlineViewButtonBox::ThemedOutlineViewButtonBox(QWidget *parent) :
        QWidget(parent),
        m_buttonIndex(-1) {

    setFixedHeight(buttonBoxHeight+borderWidth);
}

void Nedrysoft::ThemedOutlineViewButtonBox::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.drawPixmap(rect(), m_pixmap);
}

void Nedrysoft::ThemedOutlineViewButtonBox::mousePressEvent(QMouseEvent *event) {
    int xHit = event->pos().x()/buttonBoxWidth;

    if ((xHit>=0) && (xHit<2)) {
        m_buttonIndex = xHit;

        renderToPixmap(rect());

        update();
    }
}

void Nedrysoft::ThemedOutlineViewButtonBox::mouseReleaseEvent(QMouseEvent *event) {
    auto xHit = event->pos().x()/buttonBoxWidth;

    if ( (event->pos().y()>=0) &&
         (event->pos().y()<buttonBoxHeight) &&
         (xHit >= 0 ) &&
         (xHit < 2 ) ) {
            Q_EMIT buttonClicked(m_buttonIndex);
    }

    m_buttonIndex = -1;

    renderToPixmap(rect());

    update();
}

void Nedrysoft::ThemedOutlineViewButtonBox::mouseMoveEvent(QMouseEvent *event) {
    auto xHit = event->pos().x()/buttonBoxWidth;
    auto previousIndex = m_buttonIndex;

    if ( (event->pos().y()>=0) &&
         (event->pos().y()<buttonBoxHeight) &&
         (xHit >= 0 ) &&
         (xHit < 2 )) {
        m_buttonIndex = xHit;
    } else {
        m_buttonIndex = -1;
    }

    if (previousIndex!=m_buttonIndex) {
        renderToPixmap(rect());

        update();
    }
}

QSize Nedrysoft::ThemedOutlineViewButtonBox::sizeHint() {
    return QSize(buttonBoxWidth, buttonBoxHeight+borderWidth);
}

void Nedrysoft::ThemedOutlineViewButtonBox::renderToPixmap(QRect rect) {
    m_pixmap = QPixmap(rect.width(), rect.height());

    QPainter imagePainter(&m_pixmap);

    QStringList buttonList = QStringList() << "+" << "-";

    auto gradient = QLinearGradient(0, rect.top(), 0, rect.bottom());

    gradient.setColorAt(0, backgroundGradient[0]);
    gradient.setColorAt(0.5, backgroundGradient[1]);
    gradient.setColorAt(1, backgroundGradient[2]);

    auto selectedGradient = QLinearGradient(0, rect.top(), 0, rect.bottom());

    selectedGradient.setColorAt(0, backgroundGradientHighlighted[0]);
    selectedGradient.setColorAt(0.5, backgroundGradientHighlighted[1]);
    selectedGradient.setColorAt(1, backgroundGradientHighlighted[2]);

    imagePainter.fillRect(rect, gradient);

    imagePainter.setPen(borderColor);

    auto boxRect = rect;

    imagePainter.setRenderHint(QPainter::Antialiasing, false);

    imagePainter.drawRect(boxRect.adjusted(0,0, -borderWidth, -borderWidth));

    auto buttonRect = rect.adjusted(0, borderWidth,0,0);

    QFont font = QFont(imagePainter.font());

    font.setPixelSize(buttonBoxHeight);

    imagePainter.setFont(font);

    QFontMetrics fontMetrics(font);

    buttonRect.adjust(borderWidth, 0, 0, -borderWidth);

    auto currentButton = 0;

    for(auto buttonText : buttonList) {
        buttonRect.setRight(buttonRect.left()+buttonWidth-borderWidth);

        auto textRect = buttonRect.adjusted(0,0,0, -fontMetrics.descent());

        if (buttonIsHighlighted(currentButton)) {
            imagePainter.fillRect(buttonRect, selectedGradient);
        }

        imagePainter.setPen(textColour);
        imagePainter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter, buttonText);

        imagePainter.setPen(borderColor);

        textRect.setLeft(textRect.right());

        auto startPoint = QPoint(textRect.topLeft())+QPoint(0, 0);
        auto endPoint = QPoint(textRect.bottomLeft())+QPoint(0, fontMetrics.descent());

        imagePainter.drawLine(startPoint, endPoint);

        buttonRect.setLeft(buttonRect.right()+borderWidth);

        currentButton++;
    }

    imagePainter.end();
}

void Nedrysoft::ThemedOutlineViewButtonBox::resizeEvent(QResizeEvent *event) {
    renderToPixmap(rect());
}

bool Nedrysoft::ThemedOutlineViewButtonBox::buttonIsHighlighted(int buttonIndex) {
    return buttonIndex==m_buttonIndex;
}
