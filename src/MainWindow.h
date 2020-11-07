/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 05/11/2020.
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

#ifndef NEDRYSOFT_MAINWINDOW_H
#define NEDRYSOFT_MAINWINDOW_H

#include <QMainWindow>
#include <QFileOpenEvent>
#include "SplashScreen.h"
#include "Image.h"

namespace Ui {
    class MainWindow;
}

namespace Nedrysoft {
    /**
     * @brief               The MainWindow class
     *
     * @details             Provides the main window for the application.
     */
    class MainWindow :
            public QMainWindow
    {
        private:
            Q_OBJECT

        public:
            /**
             * @brief           Constructs the main window.
             *
             * @param[in]       splashScreen is a pointer to the splashscreen that was created
             *                  by the main thread at startup.
             */
            MainWindow(Nedrysoft::SplashScreen *splashScreen);

            /**
             * @brief           Destroys the main window.
             */
            ~MainWindow();

            /**
             * @brief           Handles opening links via URL.
             *
             * @param[in]       url is the requested url.
             */
            void handleOpenByUrl(const QUrl &url);

            /**
             * @brief           Returns the last instance of the class
             *
             * @returns         a pointer to the MainWindow instance.
             */
            static MainWindow *getInstance();

        protected:

            /**
             * @brief           Event handler for window close event
             *
             * @param[in]       closeEvent contains the information about the event including accpt/regect functions
             */
            virtual void closeEvent(QCloseEvent *closeEvent) override;

        private:
            /**
             * @brief           Event filter mathod
             *
             * @details         This event filter is using qApp as a target, this allows us to receive events from the
             *                  operating system such as opening registered file types or handling the regex101:// URL
             *                  scheme.
             *
             * @param[in]       obj is the object that has caused this event
             * @param[in]       event contains the details of the event, such as file name or the url path.
             *
             * @returns         true if the event is handled; otherwise false.
             */
            Q_SLOT bool eventFilter(QObject *obj, QEvent *event) override;

        private:
            Ui::MainWindow *ui;                                     //! ui class for the main window
            static MainWindow *m_instance;                          //! instance of the main window
            Image *m_backgroundImage;
            QPixmap m_backgroundPixmap;
    };
}

#endif //NEDRYSOFT_MAINWINDOW_H
