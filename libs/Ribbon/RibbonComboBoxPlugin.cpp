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

#include "RibbonComboBox.h"
#include "RibbonComboBoxPlugin.h"
#include <QtPlugin>

RibbonComboBoxPlugin::RibbonComboBoxPlugin(QObject *parent) :
        QObject(parent),
        m_initialized(false) {
}

void RibbonComboBoxPlugin::initialize(QDesignerFormEditorInterface *core) {
    Q_UNUSED(core);

    if (m_initialized) {
        return;
    }

    m_initialized = true;
}

bool RibbonComboBoxPlugin::isInitialized() const {
    return m_initialized;
}

QWidget *RibbonComboBoxPlugin::createWidget(QWidget *parent) {
    return new Nedrysoft::Ribbon::RibbonComboBox(parent);
}

QString RibbonComboBoxPlugin::name() const {
    return QStringLiteral("Nedrysoft::Ribbon::RibbonComboBox");
}

QString RibbonComboBoxPlugin::group() const {
    return QStringLiteral("Ribbon Widgets");
}

QIcon RibbonComboBoxPlugin::icon() const {
    return QIcon();
}

QString RibbonComboBoxPlugin::toolTip() const {
    return QString();
}

QString RibbonComboBoxPlugin::whatsThis() const {
    return QString();
}

bool RibbonComboBoxPlugin::isContainer() const {
    return false;
}

QString RibbonComboBoxPlugin::domXml() const {
    return "<ui language=\"c++\" displayname=\"Ribbon Combo Box\">\n"
           " <widget class=\"Nedrysoft::Ribbon::RibbonComboBox\" name=\"ribbonComboBox\">\n"

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

QString RibbonComboBoxPlugin::includeFile() const {
    return QStringLiteral("Ribbon/RibbonComboBox.h");
}
