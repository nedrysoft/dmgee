/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 21/11/2020.
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

#ifndef NEDRYSOFT_SETTINGSDIALOG_H
#define NEDRYSOFT_SETTINGSDIALOG_H

#include <QIcon>
#include <QMap>
#include <QWidget>

#if defined(Q_OS_MACOS)
class QMacToolBar;
#endif

class QHBoxLayout;
class QLabel;
class QMacToolBarItem;
class QParallelAnimationGroup;
class QPushButton;
class QStackedWidget;
class QTreeWidget;
class QVBoxLayout;

namespace Nedrysoft {
    class TransparentWidget;
    class ISettingsPage;

    /**
     * @brief       The SettingsPage class describes an individual page of the application settings
     */
    class SettingsPage {
        public:
            /**
             * @brief       The icon identifiers available for the settings page.
             */
            enum Icon {
                General,                        /**< General settings page */
                LicenceTemplates,               /**< License templates page */
                User                            /**< User settings page */
            };

        public:
            QString m_name;                     //! display name of the settings category page
            QString m_description;              //! description of the settings category page
#if defined(Q_OS_MACOS)
            TransparentWidget *m_widget;        //! the widget that contains the settings for this category
#else
            QWidget *m_widget;                  //! the widget that contains the settings for this category
#endif
            ISettingsPage *m_pageSettings;      //! pointer to the page interface
            Icon m_icon;                        //! the icon of the page
            QMacToolBarItem *m_toolBarItem;     //! toolbar item
    };

    /**
    * @brief        The SettingsDialog class provides a common themed settings dialog for Windows and Linux
    *               and a correctly styled dialog for macOS.
    *
    * @details      Describes an individual page of the application settings
    */
    class SettingsDialog :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new SettingsDialog instance which is a child of the parent.
             *
             * @param[in]   parent is the the owner of the child.
             */
            explicit SettingsDialog(QWidget *parent=nullptr);

            /**
             * @brief       Destroys the SettingsDialog.
             */
            ~SettingsDialog();

            /**
             * @brief       This signal is emitted when the window is closed by the user.
             */
            Q_SIGNAL void closed();

        protected:
            /**
             * @brief       Reimplements: QWidget::closeEvent(QCloseEvent *event).
             *
             * @param[in]   event the event information.
             */
            void closeEvent(QCloseEvent *event) override;

        private:
            /**
             * @brief       Returns the QWindow handle from native widget.
             *
             * @returns     The QWindow handle of the native window.
             */
            QWindow *nativeWindowHandle();

            /**
             * @brief       Returns the platform specific icon for the given icon type.
             *
             * @param[in]   icon the icon to get.
             *
             * @returns     the requested icon.
             */
            QIcon getIcon(SettingsPage::Icon icon);

            /**
             * @brief       Checks if the settings dialog can be closed.
             *
             * @returns     true if closable; otherwise false.
             */
            bool okToClose();

        protected:
            /**
             * @brief       Reimplements: QWidget::resizeEvent(QResizeEvent *event).
             *
             * @param[in]   event the event information.
             */
            void resizeEvent(QResizeEvent *event) override;

            /**
             * @brief       Adds a setting page to the settings dialog.
             *
             * @params[in]  section is the displayed name of the section
             * @params[in]  category is the category within the section
             * @params[in]  description is the description of the purpose of the page
             * @params[in]  icon is the icon of the page
             * @params[in]  widget is the widget containing the page content
             * @params[in]  defaultPage true if page is the default shown page; otherwise false
             *
             * @returns     the settings page structure
             */
            SettingsPage *addPage(QString section, QString category, QString description, SettingsPage::Icon icon, QWidget *widget, bool defaultPage=false);

        private:
#if defined(Q_OS_MACOS)
            QMacToolBar *m_toolBar;                             //! A native macOS toolbar (unified style)
            SettingsPage *m_currentPage;                        //! current widget
            int m_toolbarHeight;                                //! the height of the unified toolbar
            int m_maximumWidth;                                 //! the maximal width of all the settings page.
            QParallelAnimationGroup *m_animationGroup;          //! the currently active animation
#else
            QVBoxLayout *m_layout;                              //! details layout + main layout + control layout
            QVBoxLayout *m_detailLayout;                        //! detail layout
            QHBoxLayout *m_mainLayout;                          //! detail layout + tree widget
            QHBoxLayout *m_controlsLayout;                      //! apply/ok/cancel layout
            QTreeWidget *m_treeWidget;                          //! tree widget for categories
            QStackedWidget *m_stackedWidget;                    //! stacked widget for page content
            QLabel *m_categoryLabel;                            //! category label
            QPushButton *m_okButton;                            //! ok button, saves and dismisses the dialog (if changes valud)
            QPushButton *m_cancelButton;                        //! cancel button, fogets any changes made since last apply
            QPushButton *m_applyButton;                         //! apply button, saves changes but keeps dialog open
#endif
            QMap<QMacToolBarItem *, SettingsPage *> m_pages;    //! The list of settings widgets
    };
}

#endif // NEDRYSOFT_SETTINGSDIALOG_H
