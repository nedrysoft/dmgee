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

//! @note Python must be included first!

#include "Python.h"

#include "CLI/CLI.hpp"
#include "MainWindow.h"
#include "SplashScreen.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDirIterator>
#include <QFontDatabase>
#include <QMimeDatabase>
#include <QRegularExpression>

constexpr auto applicationName = APPLICATION_LONG_NAME;                     //! Provided by CMake to the preprocessor
constexpr auto applicationFontsPrefix = ":/fonts";                          //! Fonts are stored under :/fonts (recursive search is performed)

void qtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QDateTime date = QDateTime::currentDateTime();

    QMap<QtMsgType, QString> typeMap{
        {QtInfoMsg, "QtInfoMsg"},
        {QtDebugMsg, "QtDebugMsg"},
        {QtWarningMsg, "QtWarningMsg"},
        {QtCriticalMsg, "QtCriticalMsg"},
        {QtFatalMsg, "QtFatalMsg"}};

    switch (type) {
        case QtDebugMsg: {
            std::cout << localMsg.toStdString() << std::endl;
            break;
        }

        case QtInfoMsg:
        case QtWarningMsg:
        case QtCriticalMsg:
        case QtFatalMsg: {
            std::cout << "["
                      << typeMap[type].toStdString()
                      << ": " << date.toString().toStdString()
                      << "] " << localMsg.toStdString()
                      << " ("
                      << context.file
                      << ":"
                      << context.line
                      << ", "
                      << context.function
                      << std::endl;

            if (type==QtFatalMsg) {
                abort();
            }

            break;
        }
    }
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(qtMessageHandler);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication application(argc, argv);
    QMimeDatabase mimeDatabase;
    int returnValue = 0;

    qDebug() << "hello";

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

    auto configOption = appCli.add_option("-c, --config", configFilename, "the filename of the configuration file to be used to generate the DMG");
    auto outputOption = appCli.add_option("-o, --output", dmgFilename, "the filename of the created DMG. (overrides value in config file)");
    auto editOption = appCli.add_option("-e, --edit", nullptr, "Whether to open the editor (default false)");
    auto buildOption = appCli.add_option("-b, --build", nullptr, "Uses the given configuration to build the DMG");

    buildOption->needs(configOption);
    buildOption->needs(outputOption);

    editOption->required(false);

    auto versionMessage = QString("display the version of %1 and then exit").arg(APPLICATION_SHORT_NAME).toStdString();

    auto versionFlag = appCli.add_flag("-V, --version", versionMessage);

    CLI11_PARSE(appCli, argc, argv);

    if (versionFlag->count()) {
        qDebug() <<  QString::fromStdString(appCli.get_name()) << " " << versionString;

        return 0;
    }

    if ((editOption->count()) || (!buildOption->count())) {
        // search the /fonts folder in the resources and attempt to load any found fonts

        auto fontDirIterator = QDirIterator(applicationFontsPrefix, QDirIterator::Subdirectories);

        while (fontDirIterator.hasNext()) {
            fontDirIterator.next();

            auto mimeType = mimeDatabase.mimeTypeForFile(fontDirIterator.filePath()).name();

            if (QRegularExpression(R"(font\/.*)").match(mimeType).hasMatch()) {
                QFontDatabase::addApplicationFont(fontDirIterator.filePath());
            }
        }

        ilInit();

        ilEnable(IL_ORIGIN_SET);

        ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);

        auto splashScreen = Nedrysoft::SplashScreen::getInstance();

        application.setApplicationDisplayName(applicationName);
        application.setApplicationName(applicationName);

        auto mainWindow = Nedrysoft::MainWindow::getInstance();

        mainWindow->show();

        mainWindow->loadConfiguration(QString::fromStdString(configFilename));

        returnValue = application.exec();
    }

    return returnValue;
}