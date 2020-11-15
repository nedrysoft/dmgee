/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 30/10/2020.
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

#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QDialog>
#include <QFont>
#include <QFontDatabase>
#include <QPainter>
#include <QString>

constexpr auto splashScreenFilename = ":/images/splash_620x375@2x.png";
constexpr auto fontFamily = "Open Sans";
constexpr auto fontSize = 14;

Nedrysoft::AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent, Qt::FramelessWindowHint) {

    m_backgroundPixmap = QPixmap(splashScreenFilename);

    auto dialogSize = (QSizeF(m_backgroundPixmap.size())/m_backgroundPixmap.devicePixelRatioF());

    resize((dialogSize.toSize()));

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_AlwaysStackOnTop);
}

void Nedrysoft::AboutDialog::focusOutEvent(QFocusEvent *event) {
    Q_UNUSED(event);

    close();
}

bool Nedrysoft::AboutDialog::event(QEvent *event) {
    switch(event->type())
    {
        case QEvent::MouseButtonPress:
        case QEvent::KeyPress: {
            close();
            break;
        }

        default: {
            break;
        }
    }

    return QDialog::event(event);
}

void Nedrysoft::AboutDialog::paintEvent(QPaintEvent *paintEvent) {
    QDialog::paintEvent(paintEvent);

    auto font = QFont(fontFamily, fontSize, QFont::Weight::Normal);
    auto versionText = QString("%1.%2.%3 (%4 %5)").arg(APPLICATION_GIT_YEAR).arg(APPLICATION_GIT_MONTH).arg(APPLICATION_GIT_DAY).arg(APPLICATION_GIT_BRANCH).arg(APPLICATION_GIT_HASH);

    QPainter painter(this);

    painter.save();

    painter.drawPixmap(0, 0, m_backgroundPixmap);

    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.setFont(font);

    painter.drawText(QRect(350, 300, 250, 71), Qt::AlignRight | Qt::AlignVCenter, versionText);

    painter.restore();
}
