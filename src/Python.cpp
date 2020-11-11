/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 30/10/2020.
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
#include <QByteArray>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

Nedrysoft::Python::Python() {
    Py_Initialize();
}

Nedrysoft::Python::~Python() {
    Py_Finalize();
}

bool Nedrysoft::Python::run(QString &filename) {
    QFile pythonFile(filename);

    if (pythonFile.open(QFile::ReadOnly)) {
        QByteArray pythonContent = pythonFile.readAll();

        if (pythonContent.length()) {
            return runScript(QString::fromUtf8(pythonContent), nullptr);
        }
    }

    return false;
}

void Nedrysoft::Python::addModulePaths(QStringList modulePaths) {
    m_modulePaths = std::move(modulePaths);
}

bool Nedrysoft::Python::runScript(const QString& script, PyObject * locals) {
    PyObject *systemModule = PyImport_ImportModule("sys");
    PyObject *systemPath = PyObject_GetAttrString(systemModule, "path");

    // add our local packages & dependencies to load first

    for (const auto& modulePath : m_modulePaths) {
        QDirIterator dirIterator(modulePath);

        while (dirIterator.hasNext()) {

            if (dirIterator.fileName().startsWith(".")) {
                continue;
            }

            PyObject *localModulePath = PyUnicode_FromString(dirIterator.path().toLatin1().data());

            PyList_Insert(systemPath, 0, localModulePath);

            Py_DECREF(localModulePath);
        }
    }

    auto dict = PyDict_New();

    PyRun_String(script.toLatin1().data(), Py_file_input, dict, locals);

    Py_DECREF(systemModule);
    Py_DECREF(systemPath);

    if (Py_FinalizeEx() < 0) {
        return false;
    }

    //PyMem_RawFree(program);

    return true;
}