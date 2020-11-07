/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * https://github.com/fizzyade
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

#ifndef NEDRYSOFT_RIBBON_RIBBONGROUP_H
#define NEDRYSOFT_RIBBON_RIBBONGROUP_H

#include "RibbonSpec.h"
#include <QWidget>
#include <QFontMetrics>

namespace Nedrysoft::Ribbon
{
    /**
     * @brief       Ribbon Group Widget
     *
     * @details     Widget to enclose a group of controls
     *
     */
    class NEDRYSOFT_RIBBON_WIDGET_EXPORT RibbonGroup :
        public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(QString groupName READ groupName WRITE setGroupName)

    public:
        /**
         * @brief       Constructor
         *
         * @param[in]   parent          parent object
         *
         */
        explicit RibbonGroup(QWidget *parent = nullptr);

        /**
         * @brief       Gets the name of the group
         *
         * @return      name of the group
         *
         */
        QString groupName() const;

        /**
         * @brief       Sets the name of the group
         *
         * @param[in]   name            The group name
         *
         */
        void setGroupName(const QString &name);

    protected:
        /**
         * @brief       paintEvent
         *
         * @details     Overridden paintEvent for drawing widget
         *
         * @param[in]   event           The event information
         *
         */
        virtual void paintEvent(QPaintEvent *event);

    signals:

    private:
        QString m_groupName;                                    //! Group name that is displayed
        QFont m_font;                                           //! Font used to draw the group name
        QFontMetrics m_fontMetrics;                             //! The font metrics of the selected font
        QRect m_textRect;                                       //! The rectangle of the group name in the selected font

        QString m_lightStyleSheet;                              //! Light QSS style sheet
        QString m_darkStyleSheet;                               //! Dark QSS style sheet
    };
}

#endif // NEDRYSOFT_RIBBON_RIBBONGROUP_H
