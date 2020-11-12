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

#include "RibbonLineEdit.h"
#include "RibbonLineEditPlugin.h"
#include <QtPlugin>

RibbonLineEditPlugin::RibbonLineEditPlugin(QObject *parent) :
        QObject(parent),
        m_initialized(false) {
}

void RibbonLineEditPlugin::initialize(QDesignerFormEditorInterface *core) {
    Q_UNUSED(core);

    if (m_initialized)
        return;

    m_initialized = true;
}

bool RibbonLineEditPlugin::isInitialized() const {
    return m_initialized;
}

QWidget *RibbonLineEditPlugin::createWidget(QWidget *parent) {
    return new Nedrysoft::Ribbon::RibbonLineEdit(parent);
}

QString RibbonLineEditPlugin::name() const {
    return QStringLiteral("Nedrysoft::Ribbon::RibbonLineEdit");
}

QString RibbonLineEditPlugin::group() const {
    return QStringLiteral("Ribbon Widgets");
}

QIcon RibbonLineEditPlugin::icon() const {
    return QIcon();
}

QString RibbonLineEditPlugin::toolTip() const {
    return QString();
}

QString RibbonLineEditPlugin::whatsThis() const {
    return QString();
}

bool RibbonLineEditPlugin::isContainer() const {
    return false;
}

QString RibbonLineEditPlugin::domXml() const {
    return "<ui language=\"c++\" displayname=\"Ribbon Line Edit\">\n"
           " <widget class=\"Nedrysoft::Ribbon::RibbonLineEdit\" name=\"ribbonLineEdit\">\n"

           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>13</height>\n"
           "   </rect>\n"
           "  </property>\n"

           " </widget>\n"
           "</ui>\n";
}

QString RibbonLineEditPlugin::includeFile() const {
    return QStringLiteral("Ribbon/RibbonLineEdit.h");
}
