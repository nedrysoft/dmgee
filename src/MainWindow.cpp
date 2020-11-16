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
#include "ImageLoader.h"

#include <QAction>
#include <QDebug>
#include <QDesktopServices>
#include <QFileInfo>
#include <QList>
#include <QMenu>
#include <QMimeData>
#include <opencv2/opencv.hpp>
#include <QPaintEvent>
#include <QPainter>
#include <QTemporaryDir>
#include <QTimer>
#include <QWindow>
#include <utility>

using namespace std::chrono_literals;

constexpr auto splashScreenDuration = 100ms;//3s;

Nedrysoft::MainWindow *Nedrysoft::MainWindow::m_instance = nullptr;

Nedrysoft::MainWindow::MainWindow() :
        QMainWindow(nullptr),
        ui(new Ui::MainWindow),
        m_minimumPixelArea(10000),
        m_backgroundImage(),
        m_builder(new Builder) {

    ui->setupUi(this);

    qobject_cast<QApplication *>(QCoreApplication::instance())->installEventFilter(this);

    QTimer::singleShot(splashScreenDuration, []() {
        Nedrysoft::SplashScreen::getInstance()->close();
    });

    connect(ui->actionQuit, &QAction::triggered, [this](bool isChecked) {
        close();
    });

    connect(ui->actionAbout, &QAction::triggered, [](bool isChecked) {
        Nedrysoft::AboutDialog aboutDialog;

        aboutDialog.exec();
    });

    updatePixmap();

    QDesktopServices::setUrlHandler("dmgee", this, SLOT("handleOpenByUrl"));

    connect(ui->minFeatureSlider, &QSlider::valueChanged, [this](int newValue) {
        m_minimumPixelArea = newValue;

        if (ui->featureAutoDetectCheckbox->isChecked()) {
            processBackground();
        }
    });

    ui->gridVisibleCheckbox->setCheckState(configValue("gridVisible", false).toBool() ? Qt::Checked : Qt::Unchecked);
    ui->gridSnapCheckbox->setCheckState(configValue("gridShouldSnap", false).toBool() ? Qt::Checked : Qt::Unchecked);
    ui->featureAutoDetectCheckbox->setCheckState(configValue("snapToFeatures", true).toBool() ? Qt::Checked : Qt::Unchecked);
    ui->showIconsCheckBox->setCheckState(configValue("iconsVisible", true).toBool() ? Qt::Checked : Qt::Unchecked);

    connect(ui->gridVisibleCheckbox, &QCheckBox::stateChanged, [this](int state) {
        ui->previewWidget->setGrid(configValue("grid", QSize(20,20)).value<QSize>(), ( state == Qt::Checked ) ? true : false, true);
    });

    connect(ui->showIconsCheckBox, &QCheckBox::stateChanged, [this](int state) {
        ui->previewWidget->setIconsVisible(( state == Qt::Checked ) ? true : false);
    });

    connect(ui->featureAutoDetectCheckbox, &QCheckBox::stateChanged, [this](int state) {
        if (!state) {
            ui->previewWidget->clearCentroids();
        } else {
            processBackground();
        }
    });

    // design controls

    connect(ui->designFilesAddButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, [this](bool dropdown) {
        if (dropdown) {
            QMenu popupMenu;
            auto menuPos = ui->designFilesAddButton->mapToGlobal(ui->designFilesAddButton->rect().bottomLeft());

            popupMenu.addAction("Background Image...");
            popupMenu.addAction("Shortcut To Applications");
            popupMenu.addAction("Shortcut...");
            popupMenu.addAction("Icon...");

            popupMenu.exec(menuPos);
        }
    });

    // icon size controls

    ui->iconsSizeLineEdit->setValidator(new QIntValidator(16, 512));
    ui->previewWidget->setIconSize(configValue("iconSize", 64).value<int>());

    connect(ui->iconsSizeLineEdit, &QLineEdit::textChanged, [this](const QString &text) {
        bool ok = false;
        int size = text.toInt(&ok);

        if (( ok ) && ( size != 0 )) {
            setConfigValue("iconSize", size);
            ui->previewWidget->setIconSize(text.toInt());
        }
    });

    // grid controls

    ui->gridXLineEdit->setValidator(new QIntValidator(0, 100));
    ui->gridYLineEdit->setValidator(new QIntValidator(0, 100));

    connect(ui->gridSnapCheckbox, &QCheckBox::clicked, [=](bool checked) {
        setConfigValue("snapToGrid", checked);

        ui->previewWidget->setGrid(QSize(ui->gridXLineEdit->text().toInt(),ui->gridYLineEdit->text().toInt()),
                                   ui->gridVisibleCheckbox->isChecked(),
                                   ui->gridSnapCheckbox->isChecked());
    });

    connect(ui->gridVisibleCheckbox, &QCheckBox::clicked, [=](bool checked) {
        setConfigValue("gridVisible", checked);

        ui->previewWidget->setGrid(QSize(ui->gridXLineEdit->text().toInt(),ui->gridYLineEdit->text().toInt()),
                                   ui->gridVisibleCheckbox->isChecked(),
                                   ui->gridSnapCheckbox->isChecked());
    });

    // text controls

    ui->fontSizeLineEdit->setValidator(new QIntValidator(6, 72));

    connect(ui->fontSizeLineEdit, &QLineEdit::textChanged, [this](const QString &text) {
        bool ok = false;
        int size = text.toInt(&ok);

        if (( ok ) && ( size != 0 )) {
            setConfigValue("textSize", size);
            ui->previewWidget->setTextSize(text.toInt());
        }
    });

    ui->positionComboBox->addItems(QStringList() << "Bottom" << "Right");
    ui->positionComboBox->setCurrentIndex(configValue("textPosition", Nedrysoft::Builder::Bottom).value<Nedrysoft::Builder::TextPosition>());

    // process the background image (if there is one loaded) to detect

    processBackground();

    ui->previewWidget->setGrid(QSize(ui->gridXLineEdit->text().toInt(),ui->gridYLineEdit->text().toInt()),
                               ui->gridVisibleCheckbox->isChecked(),
                               ui->gridSnapCheckbox->isChecked());

    ui->featureAutoDetectCheckbox->setCheckState(Qt::Checked);

    // set up the disk image formats that are supported

    QList<QPair<QString, QString> > diskFormats;

    diskFormats << QPair<QString, QString>("UDRW", "UDIF read/write image");
    diskFormats << QPair<QString, QString>("UDRO", "UDIF read-only image");
    diskFormats << QPair<QString, QString>("UDCO", "UDIF ADC-compressed image");
    diskFormats << QPair<QString, QString>("UDZO", "UDIF zlib-compressed image");
    diskFormats << QPair<QString, QString>("UDBZ", "UDIF bzip2-compressed image (macOS 10.4+ only)");
    diskFormats << QPair<QString, QString>("UFBI", "UDIF entire image with MD5 checksum");
    diskFormats << QPair<QString, QString>("UDRo", "UDIF read-only (obsolete format)");
    diskFormats << QPair<QString, QString>("UDCo", "UDIF compressed (obsolete format)");
    diskFormats << QPair<QString, QString>("UDTO", "DVD/CD-R master for export");
    diskFormats << QPair<QString, QString>("UDxx", "UDIF stub image");
    diskFormats << QPair<QString, QString>("UDSP", "SPARSE (grows with content)");
    diskFormats << QPair<QString, QString>("UDSB", "SPARSEBUNDLE (grows with content; bundle-backed)");
    diskFormats << QPair<QString, QString>("RdWr", "NDIF read/write image (deprecated)");
    diskFormats << QPair<QString, QString>("Rdxx", "NDIF read-only image (Disk Copy 6.3.3 format)");
    diskFormats << QPair<QString, QString>("ROCo", "NDIF compressed image (deprecated)");
    diskFormats << QPair<QString, QString>("Rken", "NDIF compressed (obsolete format)");
    diskFormats << QPair<QString, QString>("DC42", "Disk Copy 4.2 image");

    for (auto format : diskFormats) {
        ui->formatComboBox->addItem(format.first);
    }

    ui->formatComboBox->setCurrentText("UDBZ");

    // build controls

    connect(ui->buildButton, &Nedrysoft::Ribbon::RibbonButton::clicked, [this]() {
        m_builder->createDMG("~/Desktop/test.dmg");
    });
}

Nedrysoft::MainWindow::~MainWindow() {
    delete ui;

    delete getInstance();
}

Nedrysoft::MainWindow *Nedrysoft::MainWindow::getInstance() {
    static Nedrysoft::MainWindow *instance = nullptr;

    if (!instance) {
        instance = new Nedrysoft::MainWindow;
    }

    return instance;
}

void Nedrysoft::MainWindow::handleOpenByUrl(const QUrl &url) {
    Q_UNUSED(url);
}

bool Nedrysoft::MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type()==QEvent::FileOpen) {
        auto fileOpenEvent = dynamic_cast<QFileOpenEvent *>(event);

        /*if (!fileOpenEvent->url().isEmpty()) {
            // TODO: launched via url scheme
        } else if (!fileOpenEvent->file().isEmpty()) {
            // TODO: launched via file association
        }*/

        return false;
    }

    return QObject::eventFilter(watched, event);
}

void Nedrysoft::MainWindow::closeEvent(QCloseEvent *closeEvent) {
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

        /*
        cv::namedWindow("Image");
        cv::imshow("Image", image);
         */

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

            if (area > m_minimumPixelArea) {
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
    if (m_builder->loadConfiguration(std::move(filename))) {
        ui->gridSnapCheckbox->setCheckState(configValue("snapToGrid", false).toBool() ? Qt::Checked : Qt::Unchecked);
        ui->gridVisibleCheckbox->setCheckState(configValue("gridVisible", false).toBool() ? Qt::Checked : Qt::Unchecked);

        ui->gridXLineEdit->setText(QString("%1").arg(configValue("gridSize", 20).toPoint().x()));
        ui->gridYLineEdit->setText(QString("%1").arg(configValue("gridSize", 20).toPoint().y()));

        ui->iconsSizeLineEdit->setText(QString("%1").arg(configValue("iconSize", 128).toInt()));
        ui->minFeatureSlider->setValue(configValue("featureSize", 10000).toInt());
        ui->fontSizeLineEdit->setText(QString("%1").arg(configValue("textSize", 12).toInt()));

        ui->featureAutoDetectCheckbox->setCheckState(configValue("detectFeatures", true).toBool() ? Qt::Checked : Qt::Unchecked);

        // add the icons from the configuration to the preview window.

        auto iconSize = configValue("iconSize", 128).toInt();
        auto files = m_builder->property("files").value<QList<Nedrysoft::Builder::File *>>();

        for (auto file : files) {
            auto applicationIcon = new Nedrysoft::Image(file->file, false, iconSize, iconSize);

            ui->previewWidget->addIcon(applicationIcon, QPoint(file->x, file->y), PreviewWidget::Icon, [=](QPoint& point){
                file->x = point.x();
                file->y = point.y();
            });
        }

        auto symlinks = m_builder->property("symlinks").value<QList<Nedrysoft::Builder::Symlink *>>();

        for (auto symlink : symlinks) {
            QTemporaryDir temporaryDir;

            if (temporaryDir.isValid()) {
                auto temporaryName = temporaryDir.path() + symlink->shortcut;

                if (QFile::link(symlink->shortcut, temporaryName)) {
                    auto applicationsShortcutImage = new Nedrysoft::Image(temporaryName, false, iconSize, iconSize);

                    ui->previewWidget->addIcon(applicationsShortcutImage, QPoint(symlink->x, symlink->y), PreviewWidget::Shortcut, [=](QPoint& point){
                        symlink->x = point.x();
                        symlink->y = point.y();
                    });
                }
            }
        }

        updatePixmap();
    }

    return true;
}

void Nedrysoft::MainWindow::updatePixmap() {
    QFileInfo fileInfo(configValue("background", "").value<QString>());

    if (!fileInfo.absoluteFilePath().isEmpty()) {
        m_backgroundImage = Nedrysoft::Image(fileInfo.absoluteFilePath(), true);

        m_backgroundPixmap = QPixmap::fromImage(m_backgroundImage.image());

        ui->previewWidget->setPixmap(m_backgroundPixmap);

        if (ui->featureAutoDetectCheckbox->isChecked()) {
            processBackground();
        }
    } else {
        m_backgroundPixmap = QPixmap();

        ui->previewWidget->setPixmap(m_backgroundPixmap);
        ui->previewWidget->clearCentroids();
    }
}

