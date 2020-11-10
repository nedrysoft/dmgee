/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * https://github.com/fizzyade
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

#ifndef NEDRYSOFT_RIBBONWIDGETPLUGIN_H
#define NEDRYSOFT_RIBBONWIDGETPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

/**
 * @brief       Provides the base Ribbon widget
 */
class RibbonWidgetPlugin :
    public QObject,
    public QDesignerCustomWidgetInterface {
        private:
            Q_OBJECT

            Q_INTERFACES(QDesignerCustomWidgetInterface)

        public:
            /**
             * @brief       Constructs a factory capable of creating RibbonBar instances
             *
             * @param[in]   parent is the owner of the factory
             */
            explicit RibbonWidgetPlugin(QObject *parent = nullptr);

            /**
             * @brief       Returns whether this factory is a container of other widgets
             *
             * @returns     true if it is a container; otherwise false
             */
            bool isContainer() const override;

            /**
             * @brief       Returns whether the plugin has been initialised
             *
             * @returns     true if initialised; otherwise false
             */
            bool isInitialized() const override;

            /**
             * @brief       Returns the icon to be used in designer for this widget
             *
             * @returns     a QIcon that depicts the widget
             */
            QIcon icon() const override;

            /**
             * @brief       Returns the XML that is inserted into the form
             *
             * @returns     a QString that contains the XML that will appear in the form description
             */
            QString domXml() const override;

            /**
             * @brief       Returns the logical group name that this widget should appear under.
             *
             * @returns     the name of the group
             */
            QString group() const override;

            /**
             * @brief       Returns the name of the include file that should be inserted into code to
             *              allow the widget to be used in a form
             *
             * @returns     the name of the include file (just the filename)
             */
            QString includeFile() const override;

            /**
             * @brief       Returns the name of this wifget
             *
             * @returns     the name of the widget
             */
            QString name() const override;

             /**
             * @brief       Returns a QString to be used as the tool tip description of the widget
             *
             * @returns     the description
             */
            QString toolTip() const override;

            /**
             * @brief       Returns a QString containing help information when the user selects "whats this" from inside designer
             *
             * @returns     the description
             */
            QString whatsThis() const override;

            /**
             * @brief       Creates a new instance of the widget
             *
             * @returns     the widget
             */
            QWidget *createWidget(QWidget *parent) override;

            /**
             * @brief       Called by designer to initialise the plugin
             */
            void initialize(QDesignerFormEditorInterface *core) override;

        private:
            bool m_initialized = false;                         //! holds whether designer has initialised the factory yet
};

#endif // NEDRYSOFT_RIBBONWIDGETPLUGIN_H
