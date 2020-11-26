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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AboutDialog.h"
#include "AnsiEscape.h"
#include "ImageLoader.h"
#include "MacHelper.h"
#include "SettingsDialog.h"
#include "ThemeSupport.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QJsonDocument>
#include <QList>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <QLocale>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QSettings>
#include <QStyleFactory>
#include <QTemporaryDir>
#include <QTimer>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWindow>
#include <utility>

// convenience macros for ansi escape sequences

#define fore Nedrysoft::AnsiEscape::fore
#define back Nedrysoft::AnsiEscape::back
#define style Nedrysoft::AnsiEscape::style
#define hyperlink Nedrysoft::AnsiEscape::link
#define reset Nedrysoft::AnsiEscape::reset()
#define underline(state) Nedrysoft::AnsiEscape::underline(state)

using namespace std::chrono_literals;

constexpr auto splashScreenDuration = 100ms;//3s;
constexpr auto repositoryUrl = "https://github.com/fizzyade/dmgee";
constexpr auto menuIconSize = 32;
constexpr auto spinnerSize = 16;

Nedrysoft::MainWindow *Nedrysoft::MainWindow::m_instance = nullptr;

// python3 -m pip install git+https://github.com/fizzyade/dmgbuild.git@master --force

Nedrysoft::MainWindow::MainWindow() :
        QMainWindow(nullptr),
        ui(new Ui::MainWindow),
        m_backgroundImage(),
        m_builder(new Builder),
        m_settingsDialog(nullptr),
        m_openRecentMenu(nullptr) {

    ui->setupUi(this);

    m_themeSupport = new Nedrysoft::Utils::ThemeSupport;

    qobject_cast<QApplication *>(QCoreApplication::instance())->installEventFilter(this);

    QDesktopServices::setUrlHandler("dmgee", this, SLOT("handleOpenByUrl"));

    initialiseLoader();

    setupStatusBar();
    setupComboBoxes();
    setupValidators();
    setupSignals();

    updateGUI();
    updateRecentFiles();

    // set up validators for fields that require one

    ui->previewWidget->setBuilder(m_builder);
    ui->previewWidget->fitToView();

    QTimer::singleShot(splashScreenDuration, []() {
        Nedrysoft::SplashScreen::getInstance()->close();
    });

    QGuiApplication::instance()->installEventFilter(this);
}

Nedrysoft::MainWindow::~MainWindow() {
    delete ui;

    if (m_openRecentMenu) {
        delete m_openRecentMenu;
    }

    delete m_themeSupport;
    delete m_progressBar;
    delete m_spinnerMovie;
    delete m_stateLabel;
    delete m_progressSpinner;
    delete m_loadingMovie;
}

Nedrysoft::MainWindow *Nedrysoft::MainWindow::getInstance() {
    static Nedrysoft::MainWindow *instance = nullptr;

    if (!instance) {
        m_instance = instance = new Nedrysoft::MainWindow;
    }

    return m_instance;
}

void Nedrysoft::MainWindow::handleOpenByUrl(const QUrl &url) {
    Q_UNUSED(url);
}

bool Nedrysoft::MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type()==QEvent::FileOpen) {
        auto fileOpenEvent = dynamic_cast<QFileOpenEvent *>(event);

        if (!fileOpenEvent->url().isEmpty()) {
            if (fileOpenEvent->url().isLocalFile()) {
                loadConfiguration(fileOpenEvent->url().toLocalFile());
            }
        } else if (!fileOpenEvent->file().isEmpty()) {
            loadConfiguration(fileOpenEvent->file());
        }
    }

    return false;
}

void Nedrysoft::MainWindow::closeEvent(QCloseEvent *closeEvent) {
    if (m_builder->modified()) {
        if (!checkAndSaveIfSaveRequired()) {
            closeEvent->ignore();

            return;
        }
    }

    if (m_settingsDialog) {
        m_settingsDialog->close();
        m_settingsDialog->deleteLater();
        m_settingsDialog = nullptr;
    }

    closeEvent->accept();
}

void Nedrysoft::MainWindow::processBackground()
{
    if (m_backgroundImage.isValid()) {
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::Mat image = m_backgroundImage.mat();

        // convert the image to grey scale for contour detection

        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

        m_centroids.clear();

        // apply thresholding

        cv::threshold(image, image, 1, 32, cv::THRESH_TRUNC);

        // apply second stage thresholding (to black and white)

        cv::threshold(image, image, 230, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        // find contours in image

        cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        // find centre of discovered objects in image

        for (auto &contour : contours) {
            float sumX = 0, sumY = 0;
            float size = contour.size();
            QPointF centroid;

            if (size > 0) {
                for (auto &point : contour) {
                    sumX += point.x;
                    sumY += point.y;
                }

                centroid = QPointF(sumX / size, sumY / size);
            }

            auto area = cv::contourArea(contour);

            if (area > configValue("featuresize", 10000).toInt()) {
                m_centroids.append(centroid);
            }
        }

        ui->previewWidget->setCentroids(m_centroids);
    }
}

bool Nedrysoft::MainWindow::setConfigValue(const QString& valueName, QVariant value) {
    if (m_builder->property(valueName.toLatin1().constData()).isValid()) {
        m_builder->setProperty(valueName.toLatin1().constData(), value);

        return true;
    }

    return false;
}

QVariant Nedrysoft::MainWindow::configValue(const QString& valueName, QVariant defaultValue) {
    if (m_builder->property(valueName.toLatin1().constData()).isValid()) {
        return m_builder->property(valueName.toLatin1().constData());
    }

    return defaultValue;
}

bool Nedrysoft::MainWindow::loadConfiguration(QString filename) {
    QFileInfo fileInfo(filename);
    QSettings settings;
    bool result = false;

    auto recentFiles = settings.value("recentFiles").toStringList();

    recentFiles.removeAll(Nedrysoft::MacHelper::normalizedPath(filename));

    filename = Nedrysoft::MacHelper::normalizedPath(filename);

    auto absoluteFilename = Nedrysoft::MacHelper::resolvedPath(filename);

    if (m_builder->loadConfiguration(absoluteFilename)) {
        ui->terminalWidget->println(fore(Qt::lightGray) +
                                    tr("Configuration file \"%1\" was loaded successfully.").arg(fore(Qt::yellow) +
                                    hyperlink(QUrl::fromLocalFile(filename).toString(), fileInfo.fileName()) +
                                    fore(Qt::lightGray)) +
                                    reset);
        result = true;

        recentFiles.push_front(Nedrysoft::MacHelper::normalizedPath(filename));
    } else {
        ui->terminalWidget->println(fore(Qt::lightGray) +
                                    tr("Configuration \"%1\" could not be loaded").arg(
                                    hyperlink(fore(Qt::yellow) + QUrl::fromLocalFile(filename).toString(), fileInfo.fileName()) +
                                    fore(Qt::lightGray)) +
                                    reset);
    }

    updateGUI();

    settings.setValue("recentFiles", recentFiles);

    updateRecentFiles();

    return true;
}

void Nedrysoft::MainWindow::updateGUI() {
    ui->gridSnapCheckbox->setCheckState(configValue("snaptogrid", false).toBool() ? Qt::Checked : Qt::Unchecked);
    ui->gridVisibleCheckbox->setCheckState(configValue("gridvisible", false).toBool() ? Qt::Checked : Qt::Unchecked);

    ui->gridXLineEdit->setText(QString("%1").arg(configValue("gridsize", 20).toSize().width()));
    ui->gridYLineEdit->setText(QString("%1").arg(configValue("gridsize", 20).toSize().height()));

    ui->iconsSizeLineEdit->setText(QString("%1").arg(configValue("iconsize", 128).toInt()));
    ui->showIconsCheckBox->setCheckState(configValue("iconsvisible", true).toBool() ? Qt::Checked : Qt::Unchecked);

    ui->fontSizeLineEdit->setText(QString("%1").arg(configValue("textsize", 12).toInt()));

    ui->featureAutoDetectCheckbox->setCheckState(configValue("detectfeatures", true).toBool() ? Qt::Checked : Qt::Unchecked);
    ui->minFeatureSlider->setValue(configValue("featuresize", 10000).toInt());

    ui->volumeNameLineEdit->setText(configValue("volumename", "My DMG").toString());

    auto textPosition = configValue("textposition", "").toString();

    if (textPosition.toLower().compare("top")) {
        ui->positionComboBox->setCurrentText("Top");
    } else {
        ui->positionComboBox->setCurrentText("Bottom");
    }

    // TODO: need to add this checkbox to ribbon
    //
    // ui->featureSnapCheckbox->setCheckState(configValue("snaptofeatures", true).toBool() ? Qt::Checked : Qt::Unchecked);

    // add the icons from the configuration to the preview window.

    updatePixmap();

    return;
}

void Nedrysoft::MainWindow::updatePixmap() {
    QFileInfo fileInfo(configValue("background", "").value<QString>());

    if (!fileInfo.absoluteFilePath().isEmpty()) {
        m_backgroundImage = Nedrysoft::Image(fileInfo.absoluteFilePath(), true);

        if (m_backgroundImage.isValid()) {
            m_backgroundPixmap = QPixmap::fromImage(m_backgroundImage.image());

            ui->previewWidget->setPixmap(m_backgroundPixmap);

            if (ui->featureAutoDetectCheckbox->isChecked()) {
                processBackground();
            }
        } else {
            m_backgroundPixmap = QPixmap();

            ui->previewWidget->setPixmap(m_backgroundPixmap);
            ui->previewWidget->clearCentroids();;
        }
    } else {
        m_backgroundPixmap = QPixmap();

        ui->previewWidget->setPixmap(m_backgroundPixmap);
        ui->previewWidget->clearCentroids();
    }

    ui->previewWidget->fitToView();
}

QString Nedrysoft::MainWindow::timespan(int milliseconds, QString &hours, QString &minutes, QString &seconds) {
    QString outputString;
    int asSeconds = milliseconds / 1000;
    int asMinutes = asSeconds / 60;
    int asHours = asMinutes / 60;

    milliseconds = milliseconds % 1000;
    asSeconds = asSeconds % 60;
    asMinutes = asMinutes % 60;

    hours.clear();
    minutes.clear();
    seconds.clear();

    if (asHours!=0) {
        outputString.append(QString(tr("%1 %2 ").arg(asHours).arg("hours")));

        hours = QString::number(asHours);
    }

    if ((asMinutes!=0) || (!outputString.isEmpty())) {
        outputString.append(QString(tr("%1 %2 ").arg(asMinutes).arg("minutes")));

        minutes = QString::number(asMinutes);
    }

    if ((asSeconds!=0) || (!outputString.isEmpty())) {
        outputString.append(QString(tr("%1 %2 ").arg(asSeconds).arg("seconds")));

        seconds = QString::number(asSeconds);
    }

    return outputString;
}

void Nedrysoft::MainWindow::onProgressUpdate(QString updateData) {
    auto updateMap = QJsonDocument::fromJson(updateData.toUtf8()).toVariant().toMap();
    QString updateMessage;
    QStringList type = updateMap["type"].toString().split("::");

    if (type[0]=="build") {
        updateMessage = handleBuildProgress(updateMap);
    } else if (type[0]=="operation") {
        updateMessage = handleOperationProgress(updateMap);
    }

    if (!updateMessage.isEmpty()) {
        int progressValue = static_cast<int>((static_cast<float>(m_progressBar->value())/static_cast<float>(m_progressBar->maximum()-1))*100);

        ui->terminalWidget->print(QString("[%1%] ").arg(progressValue, 3, 10));
        ui->terminalWidget->println(updateMessage);

        m_progressBar->setValue(m_progressBar->value()+1);
    }
}

void Nedrysoft::MainWindow::setupStatusBar() {
    m_progressBar = new QProgressBar;
    m_spinnerMovie = new QMovie;

    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        m_spinnerMovie->setFileName(":/images/spinner-dark.gif");
    } else {
        m_spinnerMovie->setFileName(":/images/spinner-light.gif");
    }

    connect(m_themeSupport, &Nedrysoft::Utils::ThemeSupport::themeChanged, [=](bool isDarkMode) {
        if (isDarkMode) {
            m_spinnerMovie->setFileName(":/images/spinner-dark.gif");
        } else {
            m_spinnerMovie->setFileName(":/images/spinner-light.gif");
        }
    });

    m_spinnerMovie->setScaledSize(QSize(spinnerSize, spinnerSize));
    m_spinnerMovie->start();

    m_progressSpinner = new QLabel();

    m_progressSpinner->setMovie(m_spinnerMovie);

    ui->statusbar->addWidget(m_progressSpinner);

    ui->statusbar->addWidget(m_progressBar);

    m_progressSpinner->setVisible(false);
    m_progressBar->setVisible(false);

    m_progressBar->setValue(0);
    m_progressBar->setRange(0, 12+m_builder->totalFiles()+m_builder->totalSymlinks());

    m_stateLabel = new QLabel(tr("Idle"));

    ui->statusbar->addPermanentWidget(m_stateLabel);
}

void Nedrysoft::MainWindow::setupComboBoxes() {
    QList<QPair<QString, QString> > diskFormats;

    diskFormats << QPair<QString, QString>("UDRW", tr("UDIF read/write image"));
    diskFormats << QPair<QString, QString>("UDRO", tr("UDIF read-only image"));
    diskFormats << QPair<QString, QString>("UDCO", tr("UDIF ADC-compressed image"));
    diskFormats << QPair<QString, QString>("UDZO", tr("UDIF zlib-compressed image"));
    diskFormats << QPair<QString, QString>("UDBZ", tr("UDIF bzip2-compressed image (macOS 10.4+ only)"));
    diskFormats << QPair<QString, QString>("UFBI", tr("UDIF entire image with MD5 checksum"));
    diskFormats << QPair<QString, QString>("UDRo", tr("UDIF read-only (obsolete format)"));
    diskFormats << QPair<QString, QString>("UDCo", tr("UDIF compressed (obsolete format)"));
    diskFormats << QPair<QString, QString>("UDTO", tr("DVD/CD-R master for export"));
    diskFormats << QPair<QString, QString>("UDxx", tr("UDIF stub image"));
    diskFormats << QPair<QString, QString>("UDSP", tr("SPARSE (grows with content)"));
    diskFormats << QPair<QString, QString>("UDSB", tr("SPARSEBUNDLE (grows with content; bundle-backed)"));
    diskFormats << QPair<QString, QString>("RdWr", tr("NDIF read/write image (deprecated)"));
    diskFormats << QPair<QString, QString>("Rdxx", tr("NDIF read-only image (Disk Copy 6.3.3 format)"));
    diskFormats << QPair<QString, QString>("ROCo", tr("NDIF compressed image (deprecated)"));
    diskFormats << QPair<QString, QString>("Rken", tr("NDIF compressed (obsolete format)"));
    diskFormats << QPair<QString, QString>("DC42", tr("Disk Copy 4.2 image"));

    for (auto format : diskFormats) {
        ui->formatComboBox->addItem(format.first);
    }

    ui->formatComboBox->setCurrentText("UDBZ");

    ui->positionComboBox->addItems(QStringList() << tr("Bottom") << tr("Right"));
    ui->positionComboBox->setCurrentIndex(configValue("textposition", Nedrysoft::Builder::Bottom).value<Nedrysoft::Builder::TextPosition>());
}

void Nedrysoft::MainWindow::onDesignFilesAddButtonClicked(bool dropdown) {
    if (dropdown) {
        QMenu popupMenu;

        auto menuPos = ui->designFilesAddButton->mapToGlobal(ui->designFilesAddButton->rect().bottomLeft());

        popupMenu.addAction(tr("Background Image..."));
        popupMenu.addAction(tr("Shortcut To Applications"));
        popupMenu.addAction(tr("Shortcut..."));
        popupMenu.addAction(tr("Icon..."));

        popupMenu.exec(menuPos);
    }
}

void Nedrysoft::MainWindow::onFeatureSliderMinimumValueChanged(int newValue) {
    setConfigValue("featuresize", newValue);

    if (ui->featureAutoDetectCheckbox->isChecked()) {
        processBackground();
    }
}

void Nedrysoft::MainWindow::onFontSizeChanged(const QString &text) {
    bool ok = false;
    int size = text.toInt(&ok);

    if (( ok ) && ( size != 0 )) {
        setConfigValue("textsize", size);
    }
}

void Nedrysoft::MainWindow::onIconSizeChanged(const QString &text) {
    bool ok = false;
    int size = text.toInt(&ok);

    if (( ok ) && ( size != 0 )) {
        setConfigValue("iconsize", size);
    }
}

void Nedrysoft::MainWindow::onAboutDialogTriggered(bool isChecked) {
    Nedrysoft::AboutDialog aboutDialog;

    aboutDialog.exec();
}

void Nedrysoft::MainWindow::onGridVisibilityChanged(int state) {
    setConfigValue("gridvisible", state);
}

void Nedrysoft::MainWindow::onIconsVisibilityChanged(int state) {
    setConfigValue("iconsvisible", state);
}

void Nedrysoft::MainWindow::onFeatureVisibilityChanged(int state) {
    if (!state) {
        ui->previewWidget->clearCentroids();
    } else {
        processBackground();
    }
}

void Nedrysoft::MainWindow::onGridSnapChanged(bool checked) {
    setConfigValue("snaptogrid", checked);
}

void Nedrysoft::MainWindow::onCreateDMG() {
    ui->terminalWidget->println("");

    // TODO: output file should be overridable from the command line

    m_builder->createDMG(m_builder->property("outputfile").toString());
}

void Nedrysoft::MainWindow::onTerminalReady() {
    auto versionText = QString("%1.%2.%3 %4 %5").arg(APPLICATION_GIT_YEAR).arg(APPLICATION_GIT_MONTH).arg(APPLICATION_GIT_DAY).arg(APPLICATION_GIT_BRANCH).arg(APPLICATION_GIT_HASH);

    ui->stackedWidget->setCurrentIndex(TerminalView::Terminal);

    ui->terminalWidget->println(
            fore(Qt::lightGray)+
            hyperlink(repositoryUrl, "dmge²")+
            fore(Qt::white)+" ("+fore("#3d96f3")+
            hyperlink(QString("%1/commit/%2").arg(repositoryUrl).arg(APPLICATION_GIT_HASH), versionText)+
            fore(Qt::white)+")"+
            reset);

    ui->terminalWidget->println("\r\n"+fore(Qt::lightGray)+tr("Ready.\r\n")+reset);
}

void Nedrysoft::MainWindow::initialiseLoader() {
    m_loadingMovie = new QMovie;

    ui->stackedWidget->setCurrentIndex(TerminalView::Loader);

    m_loadingMovie->setFileName(":/images/loading.gif");

    ui->loadingLabel->setMovie(m_loadingMovie);

    QPixmap image(m_loadingMovie->fileName());

    m_loadingMovie->setScaledSize(image.size() / 2);
    m_loadingMovie->start();
}

void Nedrysoft::MainWindow::onTerminalContextMenuTriggered() {
    QMenu menu(this);
    QIcon copyIcon, trashIcon;

    if (Nedrysoft::Utils::ThemeSupport::isDarkMode()) {
        copyIcon = QIcon(QPixmap(":/icons/copy-dark@2x.png").scaled(menuIconSize, menuIconSize));
        trashIcon = QIcon(QPixmap(":/icons/trash-dark@2x.png").scaled(menuIconSize, menuIconSize));
    } else {
        copyIcon = QIcon(QPixmap(":/icons/copy-light@2x.png").scaled(menuIconSize, menuIconSize));
        trashIcon = QIcon(QPixmap(":/icons/trash-light@2x.png").scaled(menuIconSize, menuIconSize));
    }

    auto clearTerminalAction = menu.addAction(trashIcon, tr("Clear"));
    auto copyToClipboardAction = menu.addAction(copyIcon, tr("Copy to clipboard"));

    auto selectedAction = menu.exec(QCursor::pos());

    if (selectedAction) {
        if (selectedAction == clearTerminalAction) {
            ui->terminalWidget->clear();
        } else if (selectedAction == copyToClipboardAction) {
            ui->terminalWidget->getTerminalBuffer();
        }
    }
}

void Nedrysoft::MainWindow::onTerminalUrlClicked(QString url) {
    QDesktopServices::openUrl(url);
}

void Nedrysoft::MainWindow::copyTerminalBufferToClipboard(QString terminalBuffer) {
    auto clipboard = QGuiApplication::clipboard();

    clipboard->setText(terminalBuffer);
}

QString Nedrysoft::MainWindow::handleBuildProgress(QVariantMap buildMap) {
    static QElapsedTimer durationTimer;
    bool showActivity = false;
    QStringList type = buildMap["type"].toString().split("::");
    QString updateMessage;

    if (type[1]=="started") {
        durationTimer.restart();

        updateMessage =
                fore(AnsiColour::BLUE)+
                style(AnsiStyle::BRIGHT)+
                tr("Build Started at %1.").arg(QDateTime::currentDateTime().toString())+
                reset;

        showActivity = true;

        m_stateLabel->setText(tr("Building Image..."));
        m_progressSpinner->setVisible(true);
        m_progressBar->setVisible(true);
    } else if (type[1]=="finished") {
        QString hours, minutes, seconds;
        QString hoursMinutesSecondsString;
        QString minutesSecondsString;
        QString secondsString;
        QString durationString;

        timespan(durationTimer.elapsed(), hours, minutes, seconds);

        if (!hours.isEmpty()) {
            durationString = tr("%1 hours %2 minutes %3 seconds").arg(hours).arg(minutes).arg(seconds);
        } else if (!minutes.isEmpty()) {
            durationString = tr("%1 minutes %2 seconds").arg(minutes).arg(seconds);
        } else if (!seconds.isEmpty()) {
            durationString = tr("%1 seconds").arg(seconds);
        } else {
            durationString = tr("%1 millisconds").arg(durationTimer.elapsed());
        }

        updateMessage =
                fore(AnsiColour::BLUE)+
                style(AnsiStyle::BRIGHT)+
                tr("Build Finished at %1.\r\n").arg(QDateTime::currentDateTime().toString())+
                reset+
                "\r\n"+
                fore(AnsiColour::WHITE)+
                style(AnsiStyle::BRIGHT)+
                tr("Build took %1.").arg(durationString);

        auto outputFilename = m_builder->outputFilename();

        QFileInfo fileInfo(outputFilename);

        auto humanReadablefileSize = locale().formattedDataSize(fileInfo.size());

        updateMessage +=
                "\r\n\r\n"+
                fore(AnsiColour::GREEN)+
                hyperlink(QUrl::fromLocalFile(fileInfo.absolutePath()).toString(), tr("%1 is %2, click here to reveal the DMG in finder.").arg(fileInfo.fileName()).arg(humanReadablefileSize))+
                reset;

        m_progressSpinner->setVisible(false);
        m_progressBar->setVisible(false);
        m_stateLabel->setText(tr("Idle"));
    }

    return updateMessage;
}

QString Nedrysoft::MainWindow::handleOperationProgress(QVariantMap operationMap) {
    QStringList type = operationMap["type"].toString().split("::");
    auto normalColour = fore(QColor("#A8C023"));
    QString updateMessage;

    if (type[1] == "start") {
        QStringList operation = operationMap["operation"].toString().split("::");

        updateMessage.clear();

        if (operation[0] == "settings") {
            if (operation[1] == "load") {
                updateMessage = normalColour + tr("Loading settings...") + reset;
            }
        } else if (operation[0] == "size") {
            if (operation[1] == "calculate") {
                updateMessage = normalColour + tr("Calculating DMG size...") + reset;
            }
        } else if (operation[0] == "dmg") {
            if (operation[1] == "create") {
                updateMessage = normalColour + tr("Creating DMG...") + reset;
            } else if (operation[1] == "shrink") {
                updateMessage = normalColour + tr("Shrinking DMG...") + reset;
            } else if (operation[1] == "addlicense") {
                updateMessage = normalColour + tr("Adding license...") + reset;
            }
        } else if (operation[0] == "background") {
            if (operation[1] == "create") {
                updateMessage = normalColour + tr("Creating Background Image...") + reset;
            }
        } else if (operation[0] == "files") {
            if (operation[1] == "add") {
                updateMessage = normalColour + tr("Adding files to DMG...") + reset;
            }
        } else if (operation[0] == "file") {
            if (operation[1] == "add") {
                QFileInfo fileInfo(operationMap["file"].toString());

                QString filename =
                        fore(AnsiColour::WHITE) +
                        "\"" +
                        fore(0xb0, 0x85, 0xbe) +
                        underline(true) +
                        hyperlink(QUrl::fromLocalFile(fileInfo.filePath()).toString(), fileInfo.fileName()) +
                        underline(false) +
                        fore(AnsiColour::WHITE) +
                        "\"" +
                        normalColour;

                updateMessage =
                        normalColour + QString(tr("Adding file %1...")).arg(filename) +
                        normalColour +
                        reset;
            }
        } else if (operation[0] == "symlinks") {
            if (operation[1] == "add") {
                updateMessage = normalColour + tr("Creating symlinks in DMG...") + reset;
            }
        } else if (operation[0] == "symlink") {
            if (operation[1] == "add") {
                QString filename =
                        fore(AnsiColour::WHITE) +
                        "\"" +
                        fore(0xb0, 0x85, 0xbe) +
                        underline(true) +
                        hyperlink(QUrl::fromLocalFile(operationMap["target"].toString()).toString(), operationMap["target"].toString()) +
                        underline(false) +
                        fore(AnsiColour::WHITE) +
                        "\"" +
                        normalColour;

                updateMessage =
                        normalColour +
                        QString(tr("Adding symlink %1...")).arg(filename) +
                        normalColour +
                        reset;
            }
        } else if (operation[0] == "extensions") {
            if (operation[1] == "hide") {
                updateMessage = normalColour + tr("Hiding files...") + reset;
            }
        } else if (operation[0] == "dsstore") {
            if (operation[1] == "create") {
                updateMessage = normalColour + tr("Creating DS_Store...") + reset;
            }
        }
    }

    return updateMessage;
}

void Nedrysoft::MainWindow::onPreferencesTriggered() {
    if (m_settingsDialog) {
        m_settingsDialog->raise();

        return;
    }

    m_settingsDialog = new SettingsDialog(this);

    m_settingsDialog->show();

    connect(m_settingsDialog, &SettingsDialog::closed, [=](){
        m_settingsDialog->deleteLater();

        m_settingsDialog = nullptr;
    });
}

void Nedrysoft::MainWindow::onGridSizeChanged(QString text) {
    bool xOk = false;
    bool yOk = false;

    auto x = text.toInt(&xOk);
    auto y = text.toInt(&yOk);

    if ((xOk) && (yOk)) {
        setConfigValue("gridsize", QSize(x,y));
    }
}

void Nedrysoft::MainWindow::updateRecentFiles(QString filename) {
    QSettings settings;
    int recentFileCount = 0;
    auto recentFiles = settings.value("recentFiles").toStringList();
    auto maxRecentFiles = settings.value("maxRecentFiles", 8).toInt();

    if (m_openRecentMenu) {
        m_openRecentMenu->deleteLater();
    }

    recentFiles.removeDuplicates();

    if (!filename.isNull()) {
        recentFiles.removeAll(filename);

        recentFiles.push_front(filename);

        recentFiles = recentFiles.mid(0, maxRecentFiles);

        settings.setValue("recentFiles", recentFiles);
    }

    m_openRecentMenu = new QMenu;

    for(auto recentFile : recentFiles) {
        auto resolvedFilename = Nedrysoft::MacHelper::resolvedPath(recentFile);
        auto fileInfo = QFileInfo(resolvedFilename);

        if (fileInfo.isFile()) {
            auto fileAction = new QAction(Nedrysoft::MacHelper::normalizedPath(recentFile));

            m_openRecentMenu->addAction(fileAction);

            connect(fileAction, &QAction::triggered, [this, resolvedFilename](bool checked) {
                loadConfiguration(resolvedFilename);
            });

            recentFileCount++;

            if (recentFileCount==maxRecentFiles) {
                break;
            }
        }
    }

    if (!recentFileCount) {
        auto nofilesAction = new QAction(tr("No recent files"));

        nofilesAction->setDisabled(true);

        m_openRecentMenu->addAction(nofilesAction);
    }

    auto clearAction = new QAction(tr("Clear items"));

    m_openRecentMenu->addSeparator();
    m_openRecentMenu->addAction(clearAction);

    connect(clearAction, &QAction::triggered, [=](bool checked) {
        QSettings().setValue("recentFiles", QStringList());

        updateRecentFiles();
    });

    ui->actionOpenRecent->setMenu(m_openRecentMenu);
}

void Nedrysoft::MainWindow::onLoadClicked() {
    QString defaultPath;
    auto defaultPaths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);

    if (defaultPaths.length()) {
        defaultPath = defaultPaths.at(0);
    }

    auto filename = QFileDialog::getOpenFileName(this, tr("Open Configuration"), defaultPath, tr("dmgee configuration(*.dmgee)"));

    if (!filename.isNull()) {
        if (loadConfiguration(filename)) {

        }
    }
}

void Nedrysoft::MainWindow::onNewClicked() {
    if (checkAndSaveIfSaveRequired()) {
        m_builder->clear();

        updateGUI();
    }
}

bool Nedrysoft::MainWindow::checkAndSaveIfSaveRequired() {
    if (m_builder->modified()) {
        auto messageText = tr("Configuration has not been saved.");

        if (!m_builder->filename().isEmpty()) {
            QFileInfo fileInfo(m_builder->filename());

            messageText = tr("Configuration %1 has been modified.").arg(fileInfo.fileName());
        }

        auto result = Nedrysoft::MacHelper::nativeAlert(this,
                                                    messageText,
                                                    tr("Would you like to save your changes?"),
                                                    QStringList() << tr("Save") << tr("Don't Save") << tr("Cancel"));

        switch(result) {
            case Nedrysoft::AlertButton::Button(1): {
                return saveConfiguration(false);
            }

            case Nedrysoft::AlertButton::Button(2): {
                return true;
            }

            case Nedrysoft::AlertButton::Button(3): {
                return false;
            }

            default: {
                return false;
            }
        }
    }

    return true;
}

void Nedrysoft::MainWindow::onSaveClicked() {
    saveConfiguration(false);
}

void Nedrysoft::MainWindow::onSaveAsClicked() {
    saveConfiguration(true);
}

bool Nedrysoft::MainWindow::saveConfiguration(bool saveAs) {
    auto filename = QString();

    if (( saveAs ) || ( m_builder->filename().isNull())) {
        QString defaultPath;
        QSettings settings;
        auto defaultPaths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);

        if (defaultPaths.length()) {
            defaultPath = defaultPaths.at(0);
        }

        filename = QFileDialog::getSaveFileName(this, tr("Save Configuration"), defaultPath,
                                                tr("dmgee configuration(*.dmgee)"));
        if (filename.isNull()) {
            return false;
        }
    }

    if (m_builder->saveConfiguration(filename)) {
        updateRecentFiles(Nedrysoft::MacHelper::normalizedPath(filename));

        return true;
    }

    return false;
}

void Nedrysoft::MainWindow::setupValidators() {
    ui->gridXLineEdit->setValidator(new QIntValidator(0, 100));
    ui->gridYLineEdit->setValidator(new QIntValidator(0, 100));
    ui->iconsSizeLineEdit->setValidator(new QIntValidator(16, 512));
    ui->fontSizeLineEdit->setValidator(new QIntValidator(6, 72));
}

void Nedrysoft::MainWindow::setupSignals() {
    connect(m_builder, &Nedrysoft::Builder::progressUpdate, this, &MainWindow::onProgressUpdate, Qt::QueuedConnection);

    connect(ui->terminalWidget, &Nedrysoft::HTermWidget::terminalReady, this, &MainWindow::onTerminalReady);
    connect(ui->terminalWidget, &Nedrysoft::HTermWidget::contextMenu, this, &MainWindow::onTerminalContextMenuTriggered);
    connect(ui->terminalWidget, &Nedrysoft::HTermWidget::openUrl, this, &MainWindow::onTerminalUrlClicked);
    connect(ui->terminalWidget, &Nedrysoft::HTermWidget::terminalBuffer, this, &MainWindow::copyTerminalBufferToClipboard);

    // ribbon bar

    connect(ui->fontSizeLineEdit, &QLineEdit::textChanged, this, &MainWindow::onFontSizeChanged);
    connect(ui->iconsSizeLineEdit, &QLineEdit::textChanged, this, &MainWindow::onIconSizeChanged);
    connect(ui->showIconsCheckBox, &QCheckBox::stateChanged, this, &MainWindow::onIconsVisibilityChanged);

    connect(ui->buildButton, &Nedrysoft::Ribbon::RibbonPushButton::clicked, this, &MainWindow::onCreateDMG);
    connect(ui->designFilesAddButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, this, &MainWindow::onDesignFilesAddButtonClicked);
    connect(ui->minFeatureSlider, &QSlider::valueChanged, this, &MainWindow::onFeatureSliderMinimumValueChanged);
    connect(ui->featureAutoDetectCheckbox, &QCheckBox::stateChanged, this, &MainWindow::onFeatureVisibilityChanged);

    connect(ui->gridVisibleCheckbox, &QCheckBox::stateChanged, this, &MainWindow::onGridVisibilityChanged);
    connect(ui->gridXLineEdit, &QLineEdit::textChanged, this, &MainWindow::onGridSizeChanged);
    connect(ui->gridYLineEdit, &QLineEdit::textChanged, this, &MainWindow::onGridSizeChanged);
    connect(ui->gridSnapCheckbox, &QCheckBox::clicked, this, &MainWindow::onGridSnapChanged);

    connect(ui->loadButton, &Nedrysoft::Ribbon::RibbonPushButton::clicked, this, &MainWindow::onLoadClicked);
    connect(ui->newButton, &Nedrysoft::Ribbon::RibbonPushButton::clicked, this, &MainWindow::onNewClicked);
    connect(ui->saveButton, &Nedrysoft::Ribbon::RibbonPushButton::clicked, this, &MainWindow::onSaveClicked);

    // menu actions

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onLoadClicked);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewClicked);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveClicked);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsClicked);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::onPreferencesTriggered);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAboutDialogTriggered);
}