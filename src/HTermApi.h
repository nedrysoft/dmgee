/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 18/11/2020.
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

#ifndef NEDRYSOFT_HTERMAPI_H
#define NEDRYSOFT_HTERMAPI_H

#include <QObject>

namespace Nedrysoft {
    /**
     * @brief       The HTermApi class provides a communication channel between the javascript application running in the
     *              QWebEngineView and the C++ application.  A QWebChannel is used as the transport.
     */
    class HTermApi :
        public QObject {
            private:
                Q_OBJECT

            public:
                /**
                 * @brief       Constructs a new HTermApi.
                 */
                HTermApi() = default;

                /**
                 * @brief       This method is called by the javascript after the terminal has been initialised, no
                 *              other functions may be called until this method has executed.
                 */
                Q_INVOKABLE void onTerminalReady();

                /**
                 * @brief       This signal is emitted when the terminal is ready to be used.
                 */
                Q_SIGNAL void terminalReady();

                /**
                 * @brief       This method prints the given string in the terminal.
                 *
                 * @note        Does not append a new line, use @see println which does.
                 *
                 * @param[in]   string the string to be printed.
                 */
                Q_INVOKABLE void print(QString string);

                /**
                 * @brief       This method prints the given string in the terminal and moves to the next line.
                 *
                 * @param[in]   string the string to be printed.
                 */
                Q_INVOKABLE void println(QString string);

            private:
                /**
                 * @brief       This signal is emitted by the api to print the given string to the terminal.
                 *
                 * @param[in]   string the string to be printed.
                 */
                Q_SIGNAL void printSignal(QString string);

                /**
                 * @brief       This signal is emitted by the api to print the given string to the terminal and
                 *              move to the next line.
                 *
                 * @param[in]   string the string to be printed.
                 */
                Q_SIGNAL void printlnSignal(QString string);
    };
};

#endif //NEDRYSOFT_HTERMAPI_H
