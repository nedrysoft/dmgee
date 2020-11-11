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

#include "RibbonDropButton.h"
#include "RibbonDropButtonPlugin.h"
#include <QtPlugin>

RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) :
    QObject(parent),
    m_initialized(false) {
}

void RibbonDropButtonPlugin::initialize(QDesignerFormEditorInterface *core) {
    Q_UNUSED(core);

    if (m_initialized)
        return;

    m_initialized = true;
}

bool RibbonDropButtonPlugin::isInitialized() const {
    return m_initialized;
}

QWidget *RibbonDropButtonPlugin::createWidget(QWidget *parent) {
    return new Nedrysoft::Ribbon::RibbonDropButton(parent);
}

QString RibbonDropButtonPlugin::name() const {
    return QStringLiteral("Nedrysoft::Ribbon::RibbonDropButton");
}

QString RibbonDropButtonPlugin::group() const {
    return QStringLiteral("Ribbon Widgets");
}

QIcon RibbonDropButtonPlugin::icon() const {
    return QIcon();
}

QString RibbonDropButtonPlugin::toolTip() const {
    return QString();
}

QString RibbonDropButtonPlugin::whatsThis() const {
    return QString();
}

bool RibbonDropButtonPlugin::isContainer() const {
    return true;
}

QString RibbonDropButtonPlugin::domXml() const {
    return "<ui language=\"c++\" displayname=\"Ribbon Drop Button\">\n"
           " <widget class=\"Nedrysoft::Ribbon::RibbonDropButton\" name=\"ribbonDropButton\">\n"

           "  <property name=\"groupName\">\n"
           "   <string>Group</string>\n"
           "  </property>\n"

           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"

           " </widget>\n"
           "</ui>\n";
}

QString RibbonDropButtonPlugin::includeFile() const {
    return QStringLiteral("Ribbon/RibbonDropButton.h");
}
