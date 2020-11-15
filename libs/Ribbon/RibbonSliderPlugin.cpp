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

#include "RibbonSlider.h"
#include "RibbonSliderPlugin.h"
#include <QtPlugin>

RibbonSliderPlugin::RibbonSliderPlugin(QObject *parent) :
        QObject(parent),
        m_initialized(false) {
}

void RibbonSliderPlugin::initialize(QDesignerFormEditorInterface *core) {
    Q_UNUSED(core);

    if (m_initialized) {
        return;
    }

    m_initialized = true;
}

bool RibbonSliderPlugin::isInitialized() const {
    return m_initialized;
}

QWidget *RibbonSliderPlugin::createWidget(QWidget *parent) {
    return new Nedrysoft::Ribbon::RibbonSlider(parent);
}

QString RibbonSliderPlugin::name() const {
    return QStringLiteral("Nedrysoft::Ribbon::RibbonSlider");
}

QString RibbonSliderPlugin::group() const {
    return QStringLiteral("Ribbon Widgets");
}

QIcon RibbonSliderPlugin::icon() const {
    return QIcon();
}

QString RibbonSliderPlugin::toolTip() const {
    return QString();
}

QString RibbonSliderPlugin::whatsThis() const {
    return QString();
}

bool RibbonSliderPlugin::isContainer() const {
    return false;
}

QString RibbonSliderPlugin::domXml() const {
    return "<ui language=\"c++\" displayname=\"Ribbon Slider\">\n"
           " <widget class=\"Nedrysoft::Ribbon::RibbonSlider\" name=\"ribbonSlider\">\n"

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

QString RibbonSliderPlugin::includeFile() const {
    return QStringLiteral("Ribbon/RibbonSlider.h");
}
