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

#include "Helper.h"
#include "Image.h"
#include "MacHelper.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QPoint>
#include <QRegularExpression>
#include <QStyle>
#include <QTextStream>
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

constexpr auto configurationHeader = R"(
# This file was generated by dmgee on [date]
#
# dmgee is a toool for designing and building DMG files.
#
# https://github.com/fizzyade/dmgee

)";

PyMethodDef Nedrysoft::Builder::m_moduleMethods[] = {
    {"update", (PyCFunction) Nedrysoft::Builder::update, METH_O, PyDoc_STR("provides gui with updates from python")},
    {NULL},
};

Nedrysoft::Builder::Builder() :
        m_filename(QString()),
        m_isModified(false) {

    clear();
}

QString Nedrysoft::Builder::normalisedFilename(QString filename) {
    filename = filename.replace(QRegularExpression("(^~)"), QDir::homePath());

    QFileInfo outputFileInfo(filename);

    if (outputFileInfo.isRelative()) {
        QDir rootDir(m_filename);

        rootDir.cdUp();

        return QDir::cleanPath(rootDir.absoluteFilePath(filename));
    }

    return QDir::cleanPath(filename);
}

QString Nedrysoft::Builder::outputFilename() {
    return  normalisedFilename(m_outputFilename);
}

bool Nedrysoft::Builder::createDMG(QString filename) {
    QList<QString> modulePaths;
    int imageWidth, imageHeight;

    auto dmgFilename = normalisedFilename(filename);
    auto backgroundFilename = normalisedFilename(property("background").toString());
    auto iconFilename = normalisedFilename(property("icon").toString());

    m_outputFilename = dmgFilename;

    if (QFileInfo(backgroundFilename).exists()) {
        auto backgroundImage = Nedrysoft::Image(backgroundFilename, true);

        imageWidth = static_cast<int>(backgroundImage.width());
        imageHeight = static_cast<int>(backgroundImage.height());
    } else {
        return false;
    }

    auto python = new Nedrysoft::Python();

    auto locals = PyDict_New();

    auto parameters = PyDict_New();

    PyDict_SetItemString(parameters, "volume_name", PyUnicode_FromString( property("volumename").toString().toLatin1().constData()));

    PyDict_SetItemString(parameters, "filename",
                         PyUnicode_FromString(dmgFilename.toLatin1().constData()));

    PyDict_SetItemString(parameters, "lookForHiDPI", Py_False);
    PyDict_SetItemString(parameters, "detach_retries", PyLong_FromLong(5));

    auto settings = PyDict_New();

    PyDict_SetItemString(settings, "format", PyUnicode_FromString(property("format").toString().toLatin1().constData()));
    PyDict_SetItemString(settings, "size", Py_None);
    PyDict_SetItemString(settings, "icon", PyUnicode_FromString(iconFilename.toLatin1().constData()));
    PyDict_SetItemString(settings, "background", PyUnicode_FromString(backgroundFilename.toLatin1().constData()));

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
    PyDict_SetItemString(settings, "label_pos", PyUnicode_FromString(property("textposition").toString().toLatin1().constData()));
    PyDict_SetItemString(settings, "text_size", PyLong_FromLong(property("iconsize").toInt()));
    PyDict_SetItemString(settings, "icon_size", PyLong_FromLong(property("iconsize").toFloat()/1.333333));

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
        auto absoluteFilename = normalisedFilename(file->file);
        QFileInfo currentFileInfo(absoluteFilename);

        PyList_Append(files, PyUnicode_FromString(absoluteFilename.toLatin1().data()));

        auto position = PyTuple_Pack(2, PyLong_FromLong(file->x), PyLong_FromLong(file->y));

        PyDict_SetItemString(iconLocations, currentFileInfo.fileName().toUtf8().data(), position);
    }

    for(auto symlink : m_configuration.m_symlinks) {
        auto absoluteFilename = normalisedFilename(symlink->shortcut);

        PyDict_SetItemString(symLinks, symlink->name.toLatin1().constData(), PyUnicode_FromString(absoluteFilename.toLatin1().constData()));

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

bool Nedrysoft::Builder::saveConfiguration(const QString &filename) {
    auto files = toml::array();
    auto symlinks = toml::array();

    QFileInfo fileInfo(Nedrysoft::Helper::resolvedPath(filename));
    QDir configurationFolder(Nedrysoft::Helper::resolvedPath(filename));

    for (auto currentSymlink : m_configuration.m_symlinks) {
        auto symlink = toml::table{{
                {"x", currentSymlink->x},
                {"y", currentSymlink->y},
                {"name", configurationFolder.relativeFilePath(currentSymlink->name).toStdString()},
                {"shortcut", currentSymlink->shortcut.toStdString()}
            }
        };

        symlinks.push_back(symlink);
        symlinks.push_back(symlink);

        qDebug() << Nedrysoft::Helper::resolvedPath(filename) << configurationFolder.relativeFilePath(currentSymlink->name);
    }

    for (auto currentFile : m_configuration.m_files) {
        auto file = toml::table{{
               {"x", currentFile->x},
               {"y", currentFile->y},
               {"name", configurationFolder.relativeFilePath(currentFile->file).toStdString()}
           }
        };

        files.push_back(file);
    }

    auto configuration = toml::table{{
            {"background", configurationFolder.relativeFilePath(Nedrysoft::Helper::resolvedPath(property("background").toString())).toStdString()},
            {"icon", configurationFolder.relativeFilePath(Nedrysoft::Helper::resolvedPath(property("icon").toString())).toStdString()},
            {"filename",configurationFolder.relativeFilePath(Nedrysoft::Helper::resolvedPath(property("filename").toString())).toStdString()},
            {"volumename",property("volumename").toString().toStdString()},
            {"iconsize", property("iconsize").toInt()},
            {"iconsvisible", property("iconsVisible").toBool()},
            {"detectfeatures", property("detectFeatures").toBool()},
            {"snaptofeatures", property("snaptofeatures").toBool()},
            {"textsize", property("textsize").toInt()},
            {"gridsize", toml::array{property("gridsize").toSize().width(), property("gridsize").toSize().height()}},
            {"gridvisible", property("gridvisible").toBool()},
            {"featuresize", property("featuresize").toInt()},
            {"snaptogrid", property("snaptogrid").toBool()},
            {"format", property("format").toString().toStdString()},
            {"outputfile", configurationFolder.relativeFilePath(Nedrysoft::Helper::resolvedPath(property("outputfile").toString())).toStdString()},
            {"files", files},
            {"symlinks", symlinks},
        }
    };

    QFile outputFile(filename);

    if (outputFile.open(QFile::WriteOnly)) {
        QTextStream textStream(&outputFile);
        QString header = QString::fromUtf8(configurationHeader);

        textStream << header.replace("[date]", QDateTime::currentDateTime().toString());
        textStream << QString::fromStdString((std::stringstream() << configuration).str());

        outputFile.close();
    }

    m_filename = filename;
    m_isModified = false;

    return true;
}

bool Nedrysoft::Builder::loadConfiguration(const QString& filename) {
    auto configuration = toml::parse_file(filename.toStdString());
    auto fileInfo = QFileInfo(filename);
    auto dir = QDir(fileInfo.path());

    if (configuration.failed()) {
        return false;
    }

    QList<Symlink *> symlinks;

    if (configuration["symlink"].is_array()) {
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
            symlink->shortcut = filePath;;

            symlinks.push_back(symlink);
        }
    }

    QList<File *> files;

    if (configuration["file"].is_array()) {
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

            files.push_back(file);
        }
    }

    setProperty("files", QVariant::fromValue<QList<Nedrysoft::Builder::File *>>(files));
    setProperty("symlinks", QVariant::fromValue<QList<Nedrysoft::Builder::Symlink *>>(symlinks));

    setProperty("format", QString::fromStdString(*configuration["format"].value<std::string>()));
    setProperty("iconsize", *configuration["iconsize"].value<int>());
    setProperty("textsize", *configuration["textsize"].value<int>());
    setProperty("iconsvisible", *configuration["iconsvisible"].value<bool>());
    setProperty("gridvisible", *configuration["gridvisible"].value<bool>());
    setProperty("detectfeatures", *configuration["detectfeatures"].value<bool>());

    setProperty("featuresize", *configuration["featuresize"].value<int>());

    setProperty("background", QString::fromStdString(*configuration["background"].value<std::string>()).replace(QRegularExpression("(^~)"), QDir::homePath()));
    setProperty("icon", QString::fromStdString(*configuration["icon"].value<std::string>()));
    setProperty("filename", QString::fromStdString(*configuration["filename"].value<std::string>()).replace(QRegularExpression("(^~)"), QDir::homePath()));
    setProperty("volumename", QString::fromStdString(*configuration["volumename"].value<std::string>()));
    setProperty("outputfile", QString::fromStdString(*configuration["outputfile"].value<std::string>()));

    auto textPosition = QString::fromStdString(*configuration["textposition"].value<std::string>());

    if (textPosition.toLower() == "bottom") {
        m_configuration.m_textPosition = Bottom;
    } else if (textPosition.toLower() == "right") {
        m_configuration.m_textPosition = Right;
    } else {
        m_configuration.m_textPosition = Bottom;
    }

    auto gridSize = QSize();

    if (configuration["gridsize"].is_array()) {
        auto gridSizeList = *configuration["gridsize"].as_array();

        if (gridSizeList.size() == 2) {
            gridSize = QSize(*gridSizeList[0].value<int>(), *gridSizeList[1].value<int>());
        }
    } else {
        gridSize = QSize(64, 64);
    }

    setProperty("gridsize", gridSize);

    m_filename = filename;
    m_isModified = false;

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

void Nedrysoft::Builder::clear() {
    setProperty("files", QVariant::fromValue<QList<Nedrysoft::Builder::File *>>(QList<File *>()));
    setProperty("symlinks", QVariant::fromValue<QList<Nedrysoft::Builder::Symlink *>>(QList<Symlink *>()));
    setProperty("format", "UDBZ");
    setProperty("iconsize", 64);
    setProperty("textsize", 12);
    setProperty("iconsvisible",true);
    setProperty("gridvisible", false);
    setProperty("detectfeatures", true);
    setProperty("featuresize", 10000);

    setProperty("background", "");
    setProperty("icon", "");
    setProperty("filename", "output");
    setProperty("volumename", "New DMG");
    setProperty("textposition", "Bottom");
    setProperty("outputfile", "output.dmg");

    m_configuration.m_textPosition = Right;

    setProperty("gridsize", QSize(20,20));

    m_filename = "";

    m_isModified = false;
}

void Nedrysoft::Builder::setProperty(const char *name, const QVariant &value) {
    if (this->property(name)==value)
        return;

    m_isModified = true;

    QObject::setProperty(name, value);
}

QString Nedrysoft::Builder::filename() {
    return m_filename;
}

bool Nedrysoft::Builder::modified() {
    return m_isModified;
}

void Nedrysoft::Builder::setModified(bool isModified) {
    m_isModified = isModified;
}