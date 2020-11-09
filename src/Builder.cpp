/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 07/11/2020.
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

#include "Python.h"
#include "Builder.h"
#include "Python/Python.h"
#include <optional>
#include <QPoint>

bool Nedrysoft::Builder::createDMG(toml::table table) {
    Python *python;
    std::optional<std::string> outputFilename = table["str"].value<std::string>();
    QList<QString> modulePaths;

    python = new Nedrysoft::Python();

    auto locals = PyDict_New();

    PyDict_SetItemString(locals, "filename", PyUnicode_FromString("/tmp"));

    auto *symLinks = PyList_New(1);

    PyList_SetItem(symLinks, 0, PyUnicode_FromString("/Applications"));

    PyDict_SetItemString(locals, "symlinks", symLinks);

    modulePaths.push_back("python/packages");

    python->addModulePaths(modulePaths);

    python->runScript("import sys\r\nprint(sys.path)\r\nimport os\r\nimport dmg_build\nfrom time import time,ctime\nprint('Today is', ctime(time()))\n", locals);

    delete python;

    return true;
}

bool Nedrysoft::Builder::loadConfiguration(QString filename) {
    auto configuration = toml::parse_file(filename.toStdString());

    for (toml::node &elem : *configuration["symlink"].as_array()) {
        struct Symlink symlink;
        auto entry = *elem.as_table();

        symlink.x = *entry["x"].value<int>();
        symlink.y = *entry["y"].value<int>();
        symlink.mame = QString::fromStdString(*entry["name"].value<std::string>());
        symlink.shortcut = QString::fromStdString(*entry["shortcut"].value<std::string>());

        m_configuration.m_symlinks.push_back(symlink);
    }

    for (toml::node &elem : *configuration["file"].as_array()) {
        struct File file;
        auto entry = *elem.as_table();

        file.x = *entry["x"].value<int>();
        file.y = *entry["y"].value<int>();
        file.file = QString::fromStdString(*entry["file"].value<std::string>());

        m_configuration.m_files.push_back(file);
    }

    m_configuration.m_background = QString::fromStdString(*configuration["background"].value<std::string>());
    m_configuration.m_icon = QString::fromStdString(*configuration["icon"].value<std::string>());
    m_configuration.m_filename = QString::fromStdString(*configuration["filename"].value<std::string>());
    m_configuration.m_volumename = QString::fromStdString(*configuration["volumename"].value<std::string>());
    m_configuration.m_iconsize = *configuration["iconsize"].value<int>();

    auto gridSize = *configuration["gridsize"].as_array();

    if (gridSize.size()==2) {
        m_configuration.m_gridSize = QPoint(*gridSize[0].value<int>(), *gridSize[1].value<int>());
    }

    m_configuration.m_featureSize = *configuration["featuresize"].value<int>();

    return true;
}
