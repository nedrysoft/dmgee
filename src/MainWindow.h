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
#include "ThemeSupport.h"

#include <QFileOpenEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMovie>
#include <QProgressBar>

namespace Ui {
    class MainWindow;
}

namespace Nedrysoft {
    /**
     * @brief       The MainWindow class is the main application window.
     *
     * @note        This is a singleton class, to get the instance use the getInstance() method.
     */
    class MainWindow :
        public QMainWindow {
            private:
                Q_OBJECT

            private:
                /**
                 * @brief       The terminal is contained in a stacked widget, the first page is a loading GIF animation
                 *              and the second page is the terminal, we show the animation on start and once the terminal
                 *              has fully initialised we switch to it.
                 */
                enum TerminalView {
                    Terminal = 0,                                   /**< Display Terminal. */
                    Loader                                          /**< Display Loader animation. */
                };

            private:
                /**
                 * @brief       Constructs a new MainWindow.
                 *
                 * @note        Cannot be directly instantiated, this is a singleton class and the instance can be
                 *              accessed through the getInstance() method.
                 */
                explicit MainWindow();

                /**
                 * @brief       Delete the copy constructor.
                 */
                MainWindow(const MainWindow&) = delete;

                /**
                 * @brief       Delete the assignment operator.
                 */
                MainWindow& operator=(const MainWindow&) = delete;

            public:
                /**
                 * @brief       Destroys the MainWindow.
                 */
                ~MainWindow() override;

                /**
                 * @brief       Opens the url.
                 *
                 * @param[in]   url the url to open.
                 */
                void handleOpenByUrl(const QUrl &url);

                /**
                 * @brief       Returns the instance of the MainWindow class.
                 *
                 * @returns     the MainWindow instance.
                 */
                static MainWindow *getInstance();

                /**
                 * @brief       Loads the given configuration.
                 *
                 * @note        The configuration is stored in TOML format.
                 *
                 * @param[in]   filename the name of configuration file.
                 *
                 * @returns     true if loaded ok; otherwise false.
                 */
                bool loadConfiguration(QString filename);

            protected:
                /**
                 * @brief       Reimplements: QWidget::closeEvent(QCloseEvent *event).
                 *
                 * @param[in]   event the event information.
                 */
                void closeEvent(QCloseEvent *event) override;

                /**
                 * @brief       Reimplements: QObject::eventFilter(QObject *watched, QEvent *event).
                 *
                 * @param[in]   watched the object that caused the event.
                 * @param[in]   event the event information.
                 *
                 * @returns     true if event was handled, otherwise false.
                 */
                Q_SLOT bool eventFilter(QObject *watched, QEvent *event) override;

                /**
                 * @brief       Processes the DMG background image with opencv.
                 *
                 * @details     Attempts to locate points of interest in the image which should be considered
                 *              as snap points.
                 */
                void processBackground();

                /**
                 * @brief       Loads the pixmap as specified in the configuration.
                 *
                 * @details     When the file is changed this function is called to re-load the image, after loading
                 *              it sets the preview image and runs the feature detection (if enabled).
                 */
                void updatePixmap();

                /**
                 * @brief       Returns the named value from the configuration, if the key does not exist then the function
                 *              will return the supplied default value.
                 *
                 * @param[in]   valueName the configuration key to retrieve the value for.
                 * @param[in]   defaultValue the value to be returned if the key was not found.
                 *
                 * @returns     A QVariant containing the value or default value of the key.
                 */
                QVariant configValue(const QString& valueName, QVariant defaultValue);

                /**
                 * @brief       Sets a value in the configuration.
                 *
                 * @param[in]   valueName the configuration key to set the value for.
                 * @param[in]   value the value for the given key,
                 *
                 * @returns     true if set; otherwise false.
                 */
                bool setConfigValue(const QString& valueName, QVariant value);

                /**
                 * @brief       Reimplements: QWidget::resizeEvent(QResizeEvent *event).
                 *
                 * @param[in]   event the event information.
                 */
                void resizeEvent(QResizeEvent *event) override;

            private:
                /**
                 * @brief       Returns a human readable string for an elapsed duration in milliseconds.
                 *
                 * @param[in]   milliseconds the duration.
                 *
                 * @returns     A human readable string for the elapsed duration.
                 */
                QString timespan(int milliseconds);

                /**
                 * @brief        Updates the GUI with the current progress.
                 * @param[in]    updateData the JSON update as a string.
                 */
                void onProgressUpdate(QString updateData);

                /**
                 * @brief        Sets up the controls on the status bar.
                 */
                void setupStatusBar();

                /**
                 * @brief        Sets up the disk image formats combobox.
                 */
                void setupDiskImageFormatCombo();

                /**
                 * @brief       Handles a click on the add files to design button.
                 *
                 * @param[in]   dropdown true if drop down button clicked; otherwise false.
                 */
                Q_SLOT void onDesignFilesAddButtonClicked(bool dropdown);

                /**
                 * @brief       Executes the opencv feature algorithm with the new value.
                 *
                 * @param[in]   newValue the new minimum feature size.
                 */
                Q_SLOT void onFeatureSliderMinimumValueChanged(int newValue);

                /**
                 * @brief       Called when the user changes the font size.
                 *
                 * @param[in]   text the new value that the user entered.
                 */
                Q_SLOT void onFontSizeChanged(const QString &text);

                /**
                 * @brief       Called when the user changes the icon size.
                 *
                 * @param[in]   text the new value that the user entered.
                 */
                Q_SLOT void onIconSizeChanged(const QString &text);

                /**
                 * @brief       Called when the the about dialog action is triggered.
                 *
                 * @param[in]   isChecked if the action is checked.
                 */
                Q_SLOT void onAboutDialogTriggered(bool isChecked);

                /**
                 * @brief       Called when grid visible checkbox is changed.
                 *
                 * @param[in]   state true if checked; otherwise false.
                 */
                Q_SLOT void onGridVisibilityChanged(int state);

                /**
                 * @brief       Called when icons visible checkbox is changed.
                 *
                 * @param[in]   state true if checked; otherwise false.
                 */
                Q_SLOT void onIconsVisibilityChanged(int state);

                /**
                 * @brief       Called when feature visible checkbox is changed.
                 *
                 * @param[in]   state true if checked; otherwise false.
                 */
                Q_SLOT void onFeatureVisibilityChanged(int state);

                /**
                 * @brief       Called when grid snap checkbox is changed.
                 *
                 * @param[in]   checked true if checked; otherwise false.
                 */
                Q_SLOT void onGridSnapChanged(bool checked);

                /**
                 * @brief       Called when the build button is clicked.
                 */
                Q_SLOT void onCreateDMG();

                /**
                 * @brief       Called when the hterm terminal widget has fully initialsied.
                 */
                Q_SLOT void onTerminalReady();

                /**
                 * @brief       Initialises the stacked width and displays the loader animation.
                 */
                void initialiseLoader();

                /**
                 * @brief       Called when a right click occurs in the terminal.
                 */
                Q_SLOT void onTerminalContextMenuTriggered();

                /**
                 * @brief       Called when link is clicked in the terminal.
                 */
                Q_SLOT void onTerminalUrlClicked(QString url);

                /**
                 * @brief       Copies the passed in terminal buffer to the clipboard.
                 *
                 * @param[in]   terminalBuffer the contents of the terminal buffer.
                 */
                void copyTerminalBufferToClipboard(QString terminalBuffer);

        private:
                Ui::MainWindow *ui;                                     //! ui class for the main window
                static MainWindow *m_instance;                          //! instance of the main window
                int m_minimumPixelArea;                                 //! minimum pixel area for a feature
                Image m_backgroundImage;                                //! the background image in our intermediate format
                QPixmap m_backgroundPixmap;                             //! the background image as a cached pixmap
                QList<QPointF> m_centroids;                             //! list of centroids discovered from image
                QProgressBar *m_progressBar;                            //! Progress bar when build is taking place
                Builder *m_builder;                                     //! builder instance for generating DMG
                QMovie *m_spinnerMovie;                                 //! The animated GIF used as a spinner
                QLabel *m_progressSpinner;                              //! The spinner label that is embedded in the status bar
                QLabel *m_stateLabel;                                   //! The current status of the application
                QMovie *m_loadingMovie;                                 //! The loading spinner
                Nedrysoft::Utils::ThemeSupport *m_themeSupport;         //! Theme support instance

                QVariantMap m_config;                                   //! the configuration as a variant map
    };
}

#endif //NEDRYSOFT_MAINWINDOW_H
