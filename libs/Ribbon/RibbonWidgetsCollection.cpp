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

#include "RibbonGroupPlugin.h"
#include "RibbonWidgetPlugin.h"
#include "RibbonWidgetsCollection.h"
#include "RibbonDropButtonPlugin.h"
#include "RibbonButtonPlugin.h"
#include "RibbonLineEditPlugin.h"
#include "RibbonComboBoxPlugin.h"
#include "RibbonCheckBoxPlugin.h"

RibbonWidgetsCollection::RibbonWidgetsCollection(QObject *parent) :
        QObject(parent) {

    m_widgets.append(new RibbonGroupPlugin(this));
    m_widgets.append(new RibbonWidgetPlugin(this));
    m_widgets.append(new RibbonDropButtonPlugin(this));
    m_widgets.append(new RibbonButtonPlugin(this));
    m_widgets.append(new RibbonLineEditPlugin(this));
    m_widgets.append(new RibbonComboBoxPlugin(this));
    m_widgets.append(new RibbonCheckBoxPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> RibbonWidgetsCollection::customWidgets() const {
    return m_widgets;
}
