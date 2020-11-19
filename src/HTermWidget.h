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

#ifndef NEDRYSOFT_HTERMWIDGET_H
#define NEDRYSOFT_HTERMWIDGET_H

#include "HTermApi.h"

#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QGridLayout>

namespace Nedrysoft {
    /**
     * @brief       The HTermWidget embeds a child QWebChannelView which contains the necessary HTML and Javascript
     *              to present a hterm terminal to the user.
     */
    class HTermWidget :
        public QWidget {
            private:
                Q_OBJECT

            public:
                /**
                 * @brief       Constructs a new HTermWidget instance which is a child of the parent.
                 *
                 * @param[in]   parent the owner widget.
                 */
                explicit HTermWidget(QWidget *parent=nullptr);

                /**
                 * @brief       Destroys the HTermWidget.
                 */
                ~HTermWidget();

            public:
                /**
                 * @brief       This signal is emitted when HTerm has been fully initialised.
                 */
                Q_SIGNAL void terminalReady();

                /**
                 * @brief       Prints the string to terminal without a new line.
                 *
                 * @param[in]   string the string to be printed,
                 */
                void print(QString string);

                /**
                 * @brief       Prints the string to terminal with a new line.
                 *
                 * @param[in]   string the string to be printed,
                 */
                void println(QString string);

                /**
                 * @brief       Returns whether the terminal widget is fully initialised.
                 *
                 * @note        There are two stages to iniitalisation, the first is loading the HTML into the webview
                 *              and waiting to be notified by HTerm that the terminal is ready.  Once this condition
                 *              is satisfied, the QWebChannel is created.  Once the WebChannel signals that is it ready
                 *              then we have completed initialision and signal back to c++ that the terminal can be used.
                 *
                 * @returns     true if ready; otherwise false.
                 */
                bool isReady();

            private:
                QWebEngineView *m_webEngineView;                    //! the instance of the web engine
                QGridLayout *m_layout;                              //! layout for child widgets
                QWebChannel *m_apiChannel;                          //! web channel instance for javascript<->c++ communications
                QWebEngineProfile *m_profile;                       //! the web profile we use for the terminal
                QWebEnginePage *m_page;                             //! the web page used for rendering
                HTermApi *m_terminalApi;                            //! terminal api bridge object
                bool m_terminalReady;                               //! whether the terminal is ready (fully initialised in the browser)
    };
};

#endif //NEDRYSOFT_HTERMWIDGET_H
