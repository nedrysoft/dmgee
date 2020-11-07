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

#include "Python.h"
#include "CLI/CLI.hpp"
#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include "MainWindow.h"
#include "SplashScreen.h"
#include <QDirIterator>
#include <QMimeDatabase>
#include <QRegularExpression>

constexpr auto applicationName = APPLICATION_LONG_NAME;                     //! Provided by CMake to the preprocessor
constexpr auto applicationFontsPrefix = ":/fonts";                          //! Fonts are stored under :/fonts (recursive search is performed)

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    QMimeDatabase mimeDatabase;

    CLI::App appCli("dmgee is an application for designing and creating custom DMG images.\n", APPLICATION_SHORT_NAME);

    auto versionString = QString("%1.%2.%3 (%4 %5)")
        .arg(APPLICATION_GIT_YEAR)
        .arg(APPLICATION_GIT_MONTH)
        .arg(APPLICATION_GIT_DAY)
        .arg(APPLICATION_GIT_BRANCH)
        .arg(APPLICATION_GIT_HASH
    );

    std::string configFilename;
    std::string dmgFilename;

    appCli.add_option("-c, --config", configFilename, "the filename of the configuration file to be used to generate the DMG");
    appCli.add_option("-o, --output", dmgFilename, "the filename of the created DMG. (overrides value in config file)");

    auto versionMessage = QString("display the version of %1 and then exit").arg(APPLICATION_SHORT_NAME);

    std::string versionDescription = versionMessage.toStdString();

    auto versionFlag = appCli.add_flag("-V, --version", versionDescription);

    CLI11_PARSE(appCli, argc, argv);

    if (versionFlag->count()) {
        qDebug() <<  QString::fromStdString(appCli.get_name()) << " " << versionString;

        return 0;
    }

    Nedrysoft::Python *python = new Nedrysoft::Python();

    python->runScript("print(\"Hello World\")", nullptr);

    // search the /fonts folder in the resources and attempt to load any found fonts

    auto fontDirIterator = QDirIterator(applicationFontsPrefix, QDirIterator::Subdirectories);

    while(fontDirIterator.hasNext())
    {
        fontDirIterator.next();

        auto mimeType = mimeDatabase.mimeTypeForFile(fontDirIterator.filePath()).name();

        if (QRegularExpression(R"(font\/.*)").match(mimeType).hasMatch()) {
            QFontDatabase::addApplicationFont(fontDirIterator.filePath());
        }
    }

    Nedrysoft::SplashScreen splashScreen;

    splashScreen.show();

    application.setApplicationDisplayName(applicationName);
    application.setApplicationName(applicationName);

    auto mainWindow = new Nedrysoft::MainWindow(&splashScreen);

    mainWindow->show();

    auto returnValue = application.exec();

    delete mainWindow;

    return returnValue;
}