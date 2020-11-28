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

#ifndef NEDRYSOFT_NEDRYSOFT_H
#define NEDRYSOFT_NEDRYSOFT_H

#include <QMap>
#include <QString>

#define NEDRY_PROPERTY(type,name,getter,setter)                                                             \
    private:                                                                                                \
        type m_##name;                                                                                      \
    public:                                                                                                 \
        virtual type getter()          { return m_##name; }                                                 \
        virtual void setter(type name) { this->m_##name = name; }

#define NEDRY_SETTING(type, name, getter, setter, defaultValue)                                             \
    public:                                                                                                 \
        virtual type getter()          { return m_settings.value(name, defaultValue).value<type>(); }       \
        virtual void setter(type value) { m_settings.setValue(name, value); }

#define StringMap QMap<QString, QString>
#define StringMapIterator(map) QMapIterator<QString, QString>(map)

#endif //NEDRYSOFT_NEDRYSOFT_H
