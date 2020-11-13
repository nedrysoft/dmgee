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

#include "Builder.h"
#include "Image.h"
#include "SplashScreen.h"
#include <QFileOpenEvent>
#include <QMainWindow>

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
        public QMainWindow {
            private:
                Q_OBJECT

            private:
                /**
                 * @brief           Constructs the main window.
                 */
                explicit MainWindow();

                /**
                 * @brief           Destroys the main window.
                 */
                ~MainWindow() override;

            public:
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

                /**
                 * @brief           Loads the given configuration
                 *
                 * @param[in]       filename is the name of the TOML format configuration to load
                 *
                 * @returns         true if loaded ok; otherwise false.
                 */
                bool loadConfiguration(QString filename);

            protected:
                /**
                 * @brief           Event handler for window close event
                 *
                 * @param[in]       closeEvent contains the information about the event including accept/reject functions
                 */
                void closeEvent(QCloseEvent *closeEvent) override;

                /**
                 * @brief           Event filter method
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

                /**
                 * @brief           Processes the DMG background image with open cv
                 *
                 * @details         Attempts to locate points of interest in the image which should be considered
                 *                  as snap points.
                 */
                void processBackground();

                /**
                 * @brief           Loads the pixmap from the configuration and sets the preview image & feature detection image
                 */
                void updatePixmap();

                /**
                 * @brief           Returns the named value from the configuration, if the key does not exist then the function
                 *                  will return the supplied default value.
                 *
                 * @param[in]       valueName is the configuration key to retrieve the value for
                 * @param[in]       defaultValue is the value to be returned if the key was not found
                 *
                 * @returns         A QVariant containing the value or default value of the key
                 */
                QVariant configValue(const QString& valueName, QVariant defaultValue);

        private:
                Ui::MainWindow *ui;                                     //! ui class for the main window
                static MainWindow *m_instance;                          //! instance of the main window
                int m_minimumPixelArea;                                 //! minimum pixel area for a feature
                Image m_backgroundImage;                                //! the background image in our intermediate format
                QPixmap m_backgroundPixmap;                             //! the background image as a cached pixmap
                QList<QPointF> m_centroids;                             //! list of centroids discovered from image

                QSize m_grid;                                           //! grid size
                bool m_gridIsVisible;                                   //! grid visibility
                bool m_gridShouldSnap;                                  //! whether the grid should be used to snap to
                bool m_snapToFeatures;                                  //! whether features should be snapped to
                bool m_showIcons;                                       //! whether the icons for applications & shortcuts are visible
                Builder *m_builder;                                     //! builder instance for generating DMG

                QVariantMap m_config;                                   //! the configuration as a variant map
    };
}

#endif //NEDRYSOFT_MAINWINDOW_H
