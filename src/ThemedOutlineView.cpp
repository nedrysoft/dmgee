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

#include "ThemedOutlineView.h"

#include "ThemedOutlineViewButtonBox.h"

#include <QDebug>
#include <QResizeEvent>

constexpr auto borderWidth = 1;
constexpr auto borderColor = qRgb(0x21,0x21,0x21);

Nedrysoft::ThemedOutlineView::ThemedOutlineView(QWidget *parent) :
        QWidget(parent),
        m_treeViewWidget(new QTreeView),
        m_layout(new QVBoxLayout),
        m_buttonBox(new ThemedOutlineViewButtonBox) {

    m_layout->addWidget(m_treeViewWidget);

    m_layout->addWidget(m_buttonBox);

    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    setFixedWidth(200);

    setLayout(m_layout);

    connect(m_treeViewWidget, &QTreeView::clicked, this, &Nedrysoft::ThemedOutlineView::clicked);
    connect(m_buttonBox, &Nedrysoft::ThemedOutlineViewButtonBox::buttonClicked, this, &Nedrysoft::ThemedOutlineView::buttonClicked);

    m_treeViewWidget->sortByColumn(0, Qt::AscendingOrder);
    m_treeViewWidget->setSortingEnabled(true);

    m_treeViewWidget->setHeaderHidden(true);
}

void Nedrysoft::ThemedOutlineView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.drawPixmap(rect(), m_pixmap);
}

void Nedrysoft::ThemedOutlineView::resizeEvent(QResizeEvent *event) {
    renderToPixmap(rect());
}

void Nedrysoft::ThemedOutlineView::renderToPixmap(QRect rect) {
    m_pixmap = QPixmap(rect.width(), rect.height());

    QPainter imagePainter(&m_pixmap);

    rect.adjust(0, 0, -borderWidth, -borderWidth);

    imagePainter.fillRect(rect, palette().color(QPalette::Base));
    imagePainter.setPen(borderColor);
    imagePainter.drawRect(rect);

    imagePainter.end();
}

void Nedrysoft::ThemedOutlineView::setModel(QAbstractItemModel *model) {
    m_treeViewWidget->setModel(model);
}
