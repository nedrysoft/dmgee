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
#include "Image.h"
#include "Python/Python.h"
#include <optional>
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QPoint>
#include <QStyle>

bool Nedrysoft::Builder::createDMG(QString outputFilename) {
    QList<QString> modulePaths;
    int imageWidth, imageHeight;

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

    if (outputFilename.isNull()) {
        PyDict_SetItemString(parameters, "filename",
                             PyUnicode_FromString(m_configuration.m_filename.toLatin1().constData()));
    } else {
        PyDict_SetItemString(parameters, "filename",
                             PyUnicode_FromString(outputFilename.toLatin1().constData()));
    }

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

    auto titleBarHeight = qobject_cast<QApplication *>(QApplication::instance())->style()->pixelMetric(QStyle::PM_TitleBarHeight);    qDebug() << "titlebarheight: " <<  titleBarHeight;

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
        QFileInfo currentfileInfo(file->file);

        PyList_Append(files, PyUnicode_FromString(file->file.toLatin1().data()));

        auto position = PyTuple_Pack(2, PyLong_FromLong(file->x), PyLong_FromLong(file->y));

        PyDict_SetItemString(iconLocations, currentfileInfo.fileName().toUtf8().data(), position);
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

    python->addModulePaths(modulePaths);

    python->runScript("import sys\n"
                             "print(sys.path)\n"
                             "import os\n"
                             "import dmg_build\n"
                             "from time import time,ctime\n"
                             "print(settings)\n"
                             "print(parameters)\n"
                             "dmg_build.build_dmg(volume_name=parameters[\"volume_name\"],\n"
                             "                   filename=parameters[\"filename\"],\n"
                             "                   settings=settings,\n"
                             "                   lookForHiDPI=parameters[\"lookForHiDPI\"],\n"
                             "                   detach_retries=parameters[\"detach_retries\"])\n"
                             "print('Today is', ctime(time()))\n",
                      locals);

    //delete python;

    return true;
}

bool Nedrysoft::Builder::loadConfiguration(const QString& filename) {
    auto configuration = toml::parse_file(filename.toStdString());

    m_configuration.m_symlinks.clear();
    m_configuration.m_files.clear();

    for (toml::node &elem : *configuration["symlink"].as_array()) {
        auto symlink = new Symlink;
        auto entry = *elem.as_table();

        symlink->x = *entry["x"].value<int>();
        symlink->y = *entry["y"].value<int>();
        symlink->name = QString::fromStdString(*entry["name"].value<std::string>());
        symlink->shortcut = QString::fromStdString(*entry["shortcut"].value<std::string>());

        m_configuration.m_symlinks.push_back(symlink);
    }

    for (toml::node &elem : *configuration["file"].as_array()) {
        auto file = new File;
        auto entry = *elem.as_table();

        file->x = *entry["x"].value<int>();
        file->y = *entry["y"].value<int>();
        file->file = QString::fromStdString(*entry["file"].value<std::string>());

        m_configuration.m_files.push_back(file);
    }

    m_configuration.m_format = QString::fromStdString(*configuration["format"].value<std::string>());
    m_configuration.m_background = QString::fromStdString(*configuration["background"].value<std::string>());
    m_configuration.m_icon = QString::fromStdString(*configuration["icon"].value<std::string>());
    m_configuration.m_filename = QString::fromStdString(*configuration["filename"].value<std::string>());
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