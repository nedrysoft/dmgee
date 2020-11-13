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

#ifndef NEDRYSOFT_PYTHON_H
#define NEDRYSOFT_PYTHON_H

#include <QString>
#include <QStringList>
#include <QObject>

//! @note these must be included in this order otherwise compilation will fail

#include <list>
#include <string>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Nedrysoft {
    /**
     * @brief       Convenience class for executing python code using libpython
     */
    class Python :
        public QObject {
            public:
                Q_OBJECT

                enum ErrorCode {
                    Ok,
                    ScriptNotFound,
                    ScriptInvalid
                };

            public:
                /**
                 * @brief       Constructs a python instance
                 *
                 * @note        The python instance is bound to the thread that constructed it, do not call any other functions
                 *              from a different thread as this will result in crash.
                 */
                Python();

                /**
                 * @brief       Destroys a python instance
                 *
                 * @param[in]   argc is the argument count passed into main()
                 * @param[in]   argv is the argument vector passed into main()
                 */
                ~Python();

                /**
                 * @brief       Loads the script from disk and then executes it.
                 *
                 * @note        Python script is executed in a separate thread as to not block the UI, the object will
                 *              emit the finished signal with an error code once the script has finished.
                 *
                 * @param[in]   filename is the path to the script to execute
                 */
                void run(QString &filename);

                /**
                 * @brief       Runs the given script source
                 *
                 * @note        Python script is executed in a separate thread as to not block the UI, the object will
                 *              emit the finished signal with an error code once the script has finished.
                 *
                 * @param[in]   script is the python script to execute.
                 * @param[in]   locals is an object that contain local variables exposed to python
                 */
                void runScript(const QString &script, PyObject *locals);

                /**
                 * @brief       Inserts paths to local python modules to override system libraries
                 *
                 * @param[in]   modulePaths is a list of paths to insert
                 */
                void addModulePaths(QStringList modulePaths);

            public:
                /**
                 * @brief       Emitted when the thread has finished running
                 *
                 * @param[in]   result is 0 if no error occurred; otherwise it indicates an error occured.
                 * @param[in]   pythonResult is the error code from python if applicable.
                 */
                Q_SIGNAL void finished(int result, int pythonResult);

            private:
                QStringList m_modulePaths;           //! list of extra paths to search for modules in
    };
};

#endif //NEDRYSOFT_PYTHON_H
