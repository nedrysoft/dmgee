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

#include "RibbonWidget.h"
#include "RibbonWidgetPlugin.h"
#include <QtPlugin>

RibbonWidgetPlugin::RibbonWidgetPlugin(QObject *parent)
    : QObject(parent) {

    qRegisterMetaType<RibbonWidgetPlugin *>("RibbonWidgetPlugin");
}

void RibbonWidgetPlugin::initialize(QDesignerFormEditorInterface *core) {
    Q_UNUSED(core)

    if (initialized)
        return;

    initialized = true;
}

bool RibbonWidgetPlugin::isInitialized() const {
    return initialized;
}

QWidget *RibbonWidgetPlugin::createWidget(QWidget *parent) {
    return new Nedrysoft::Ribbon::RibbonWidget(parent);
}

QString RibbonWidgetPlugin::name() const {
    return QStringLiteral("Nedrysoft::Ribbon::RibbonWidget");
}

QString RibbonWidgetPlugin::group() const {
    return QStringLiteral("Ribbon Widgets");
}

QIcon RibbonWidgetPlugin::icon() const {
    return QIcon();
}

QString RibbonWidgetPlugin::toolTip() const {
    return QString();
}

QString RibbonWidgetPlugin::whatsThis() const {
    return QString();
}

bool RibbonWidgetPlugin::isContainer() const {
    return false;
}

QString RibbonWidgetPlugin::domXml() const {
    return "<ui language=\"c++\" displayname=\"Ribbon Widget\">\n"
           " <widget class=\"Nedrysoft::Ribbon::RibbonWidget\" name=\"ribbonWidget\">\n"
           " </widget>\n"
           "</ui>\n";
}

QString RibbonWidgetPlugin::includeFile() const {
    return QStringLiteral("Ribbon/RibbonWidget.h");
}
