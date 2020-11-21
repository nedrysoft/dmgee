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

#include "Python.h"                         //! @note must be included first
#include "Builder.h"
#include "Image.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QPoint>
#include <QRegularExpression>
#include <QStyle>
#include <optional>

constexpr auto BuildScript = R"(
# settings is passed in from C++.  See dmgbuild for more information

import sys
import os
import dmgbuild

def dmg_callback(data):
    import dmgee
    import simplejson

    dmgee.update(simplejson.dumps(data))

dmgbuild.build_dmg(volume_name=parameters["volume_name"],
                       filename=parameters["filename"],
                       settings=settings,
                       lookForHiDPI=parameters["lookForHiDPI"],
                       detach_retries=parameters["detach_retries"],
                       callback=dmg_callback)
)";

PyMethodDef Nedrysoft::Builder::m_moduleMethods[] = {
        {"update", (PyCFunction) Nedrysoft::Builder::update, METH_O, PyDoc_STR("provides gui with updates from python")},
        {NULL},
};

bool Nedrysoft::Builder::createDMG(QString outputFilename) {
    QList<QString> modulePaths;
    int imageWidth, imageHeight;

    outputFilename = outputFilename.replace(QRegularExpression("(^~)"), QDir::homePath());

    QFileInfo outputFileInfo(outputFilename.isEmpty() ? m_configuration.m_filename : outputFilename);

    QFileInfo fileInfo(m_configuration.m_background);

    if (!fileInfo.absoluteFilePath().isEmpty()) {
        auto backgroundImage = Nedrysoft::Image(fileInfo.absoluteFilePath(), true);

        imageWidth = static_cast<int>(backgroundImage.width());
        imageHeight = static_cast<int>(backgroundImage.height());
    } else {
        return false;
    }

    auto python = new Nedrysoft::Python();

    auto locals = PyDict_New();

    auto parameters = PyDict_New();

    PyDict_SetItemString(parameters, "volume_name", PyUnicode_FromString( m_configuration.m_volumename.toLatin1().constData()));

    PyDict_SetItemString(parameters, "filename",
                         PyUnicode_FromString(outputFileInfo.absoluteFilePath().toLatin1().constData()));

    PyDict_SetItemString(parameters, "lookForHiDPI", Py_False);
    PyDict_SetItemString(parameters, "detach_retries", PyLong_FromLong(5));

    auto settings = PyDict_New();

    PyDict_SetItemString(settings, "format", PyUnicode_FromString(m_configuration. m_format.toLatin1().constData()));
    PyDict_SetItemString(settings, "size", Py_None);
    PyDict_SetItemString(settings, "icon", PyUnicode_FromString(m_configuration.m_icon.toLatin1().constData()));
    PyDict_SetItemString(settings, "background", PyUnicode_FromString(m_configuration.m_background.toLatin1().constData()));

    PyDict_SetItemString(settings, "show_status_bar", Py_False);
    PyDict_SetItemString(settings, "show_tab_view", Py_False);
    PyDict_SetItemString(settings, "show_toolbar", Py_False);
    PyDict_SetItemString(settings, "show_pathbar", Py_False);
    PyDict_SetItemString(settings, "show_sidebar", Py_False);

    PyDict_SetItemString(settings, "sidebar_width", PyLong_FromLong(180));

    auto titleBarHeight = qobject_cast<QApplication *>(QApplication::instance())->style()->pixelMetric(QStyle::PM_TitleBarHeight);

    auto windowRectOrigin = PyTuple_Pack(2, PyLong_FromLong(0), PyLong_FromLong(0));
    auto windowRectSize = PyTuple_Pack(2, PyLong_FromLong(imageWidth), PyLong_FromLong(imageHeight+titleBarHeight));

    auto windowRect = PyTuple_Pack(2, windowRectOrigin, windowRectSize);

    PyDict_SetItemString(settings, "window_rect", windowRect);

    PyDict_SetItemString(settings, "default_view", PyUnicode_FromString("icon-view"));
    PyDict_SetItemString(settings, "show_icon_preview", Py_False);

    PyDict_SetItemString(settings, "include_icon_view_settings", PyUnicode_FromString("auto"));
    PyDict_SetItemString(settings, "include_list_view_settings", PyUnicode_FromString("auto"));

    PyDict_SetItemString(settings, "arrange_by", Py_None);

    auto gridOffset = PyTuple_Pack(2, PyLong_FromLong(0), PyLong_FromLong(0));

    PyDict_SetItemString(settings, "grid_offset", gridOffset);
    PyDict_SetItemString(settings, "grid_spacing", PyLong_FromLong(10));
    PyDict_SetItemString(settings, "label_pos", PyUnicode_FromString("bottom"));
    PyDict_SetItemString(settings, "text_size", PyLong_FromLong(16));
    PyDict_SetItemString(settings, "icon_size", PyLong_FromLong(m_configuration.m_iconsize/1.333333));

    PyDict_SetItemString(settings, "list_icon_size", PyLong_FromLong(16));
    PyDict_SetItemString(settings, "list_text_size", PyLong_FromLong(12));

    auto listScrollPosition = PyTuple_Pack(2, PyLong_FromLong(0), PyLong_FromLong(0));

    PyDict_SetItemString(settings, "list_scroll_position", listScrollPosition);
    PyDict_SetItemString(settings, "list_sort_by", PyUnicode_FromString("name"));
    PyDict_SetItemString(settings, "list_use_relative_dates", Py_True);
    PyDict_SetItemString(settings, "list_calculate_all_sizes", Py_False);

    auto listColumns = PyTuple_Pack(5,
                                    PyUnicode_FromString("name"),
                                    PyUnicode_FromString("date-modified"),
                                    PyUnicode_FromString("size"),
                                    PyUnicode_FromString("kind"),
                                    PyUnicode_FromString("date-added"));

    PyDict_SetItemString(settings, "list_columns", listColumns);

    auto listColumnWidths = PyDict_New();

    PyDict_SetItemString(listColumnWidths, "name", PyLong_FromLong(300));
    PyDict_SetItemString(listColumnWidths, "date-modified", PyLong_FromLong(181));
    PyDict_SetItemString(listColumnWidths, "date-created", PyLong_FromLong(181));
    PyDict_SetItemString(listColumnWidths, "date-added", PyLong_FromLong(181));
    PyDict_SetItemString(listColumnWidths, "date-last-opened", PyLong_FromLong(181));
    PyDict_SetItemString(listColumnWidths, "size", PyLong_FromLong(97));
    PyDict_SetItemString(listColumnWidths, "kind", PyLong_FromLong(115));
    PyDict_SetItemString(listColumnWidths, "label", PyLong_FromLong(100));
    PyDict_SetItemString(listColumnWidths, "version", PyLong_FromLong(75));
    PyDict_SetItemString(listColumnWidths, "comments", PyLong_FromLong(300));

    PyDict_SetItemString(settings, "list_column_widths", listColumnWidths);

    auto listColumnSortDirections = PyDict_New();

    PyDict_SetItemString(listColumnSortDirections, "name", PyUnicode_FromString("ascending"));
    PyDict_SetItemString(listColumnSortDirections, "date-modified", PyUnicode_FromString("descending"));
    PyDict_SetItemString(listColumnSortDirections, "date-created", PyUnicode_FromString("descending"));
    PyDict_SetItemString(listColumnSortDirections, "date-added", PyUnicode_FromString("descending"));
    PyDict_SetItemString(listColumnSortDirections, "date-last-opened", PyUnicode_FromString("descending"));
    PyDict_SetItemString(listColumnSortDirections, "size", PyUnicode_FromString("auto"));
    PyDict_SetItemString(listColumnSortDirections, "kind", PyUnicode_FromString("ascending"));
    PyDict_SetItemString(listColumnSortDirections, "label", PyUnicode_FromString("ascending"));
    PyDict_SetItemString(listColumnSortDirections, "version", PyUnicode_FromString("ascending"));
    PyDict_SetItemString(listColumnSortDirections, "comments", PyUnicode_FromString("ascending"));

    PyDict_SetItemString(settings, "list_column_sort_directions", listColumnSortDirections);

    auto symLinks = PyDict_New();
    auto files = PyList_New(0);
    auto iconLocations = PyDict_New();

    for(auto file : m_configuration.m_files) {
        QFileInfo currentFileInfo(file->file);

        PyList_Append(files, PyUnicode_FromString(file->file.toLatin1().data()));

        auto position = PyTuple_Pack(2, PyLong_FromLong(file->x), PyLong_FromLong(file->y));

        PyDict_SetItemString(iconLocations, currentFileInfo.fileName().toUtf8().data(), position);
    }

    for(auto symlink : m_configuration.m_symlinks) {
        PyDict_SetItemString(symLinks, symlink->name.toLatin1().constData(), PyUnicode_FromString(symlink->shortcut.toLatin1().constData()));

        auto position = PyTuple_Pack(2, PyLong_FromLong(symlink->x), PyLong_FromLong(symlink->y));

        PyDict_SetItemString(iconLocations, symlink->name.toLatin1().constData(), position);
    }

    PyDict_SetItemString(settings, "icon_locations", iconLocations);
    PyDict_SetItemString(settings, "files", files);
    PyDict_SetItemString(settings, "symlinks", symLinks);

    PyDict_SetItemString(locals, "settings", settings);
    PyDict_SetItemString(locals, "parameters", parameters);

    modulePaths.push_back("python/packages");

    //python->addModulePaths(modulePaths);
    python->addModule("dmgee", m_moduleMethods);

    python->setVariable("builderInstance", this);

    python->runScript(BuildScript, locals);

    return true;
}

bool Nedrysoft::Builder::loadConfiguration(const QString& filename) {
    auto configuration = toml::parse_file(filename.toStdString());
    auto fileInfo = QFileInfo(filename);
    auto dir = QDir(fileInfo.path());

    m_configuration.m_symlinks.clear();
    m_configuration.m_files.clear();

    for (toml::node &elem : *configuration["symlink"].as_array()) {
        auto symlink = new Symlink;
        auto entry = *elem.as_table();
        auto filePath = QString::fromStdString(*entry["shortcut"].value<std::string>());

        filePath = filePath.replace(QRegularExpression("(^~)"), QDir::homePath());

        if (QFileInfo(filePath).isRelative()) {
            filePath = dir.absoluteFilePath(filePath);
        }

        symlink->x = *entry["x"].value<int>();
        symlink->y = *entry["y"].value<int>();
        symlink->name = QString::fromStdString(*entry["name"].value<std::string>());
        symlink->shortcut =  filePath;;

        m_configuration.m_symlinks.push_back(symlink);
    }

    for (toml::node &elem : *configuration["file"].as_array()) {
        auto file = new File;
        auto entry = *elem.as_table();
        auto filePath = QString::fromStdString(*entry["file"].value<std::string>());

        filePath = filePath.replace(QRegularExpression("(^~)"), QDir::homePath());

        if (QFileInfo(filePath).isRelative()) {
            filePath = dir.absoluteFilePath(filePath);
        }

        file->x = *entry["x"].value<int>();
        file->y = *entry["y"].value<int>();
        file->file = filePath;

        m_configuration.m_files.push_back(file);
    }

    m_configuration.m_format = QString::fromStdString(*configuration["format"].value<std::string>());
    m_configuration.m_background = QString::fromStdString(*configuration["background"].value<std::string>()).replace(QRegularExpression("(^~)"), QDir::homePath());;
    m_configuration.m_icon = QString::fromStdString(*configuration["icon"].value<std::string>());
    m_configuration.m_filename = QString::fromStdString(*configuration["filename"].value<std::string>()).replace(QRegularExpression("(^~)"), QDir::homePath());;
    m_configuration.m_volumename = QString::fromStdString(*configuration["volumename"].value<std::string>());
    m_configuration.m_iconsize = *configuration["iconsize"].value<int>();
    m_configuration.m_textSize = *configuration["textsize"].value<int>();

    auto textPosition = QString::fromStdString(*configuration["textPosition"].value<std::string>());

    if (textPosition.toLower() == "bottom") {
        m_configuration.m_textPosition = Bottom;
    } else if (textPosition.toLower() == "right") {
        m_configuration.m_textPosition = Right;
    } else {
        // TODO: some error handling
    }

    auto gridSize = *configuration["gridsize"].as_array();

    if (gridSize.size()==2) {
        m_configuration.m_gridSize = QPoint(*gridSize[0].value<int>(), *gridSize[1].value<int>());
    }

    m_configuration.m_featureSize = *configuration["featuresize"].value<int>();

    return true;
}

void Nedrysoft::Builder::setSymlinks(QList<Nedrysoft::Builder::Symlink *> symlinks) {
    m_configuration.m_symlinks = symlinks;
}

QList<Nedrysoft::Builder::Symlink *> Nedrysoft::Builder::symlinks() {
    return m_configuration.m_symlinks;
}

void Nedrysoft::Builder::setFiles(QList<Nedrysoft::Builder::File *> files) {
    m_configuration.m_files = files;
}

QList<Nedrysoft::Builder::File *> Nedrysoft::Builder::files() {
    return m_configuration.m_files;
}

PyObject* Nedrysoft::Builder::update(PyObject *self, PyObject *updateData)
{
    auto unicodeString = PyUnicode_AsUCS4Copy(updateData);
    auto builderInstance = static_cast<Nedrysoft::Builder *>(Python::variable("builderInstance"));

    if (unicodeString) {
        auto jsonString = QString::fromUcs4(unicodeString);

        PyMem_Free(unicodeString);

        Q_EMIT builderInstance->progressUpdate(jsonString);

        return Py_True;
    }

    return Py_False;
}

int Nedrysoft::Builder::totalFiles() {
    return m_configuration.m_files.length();
}

int Nedrysoft::Builder::totalSymlinks() {
    return m_configuration.m_symlinks.length();
}