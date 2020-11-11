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

#include <opencv2/opencv.hpp>

#include "AboutDialog.h"
#include "ImageLoader.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QAction>
#include <QDebug>
#include <QDesktopServices>
#include <QMenu>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QWindow>
#include <QFileInfo>
#include <QMimeData>
#include <QTemporaryDir>
#include <utility>

using namespace std::chrono_literals;

constexpr auto splashScreenDuration = 100ms;//3s;

Nedrysoft::MainWindow *Nedrysoft::MainWindow::m_instance = nullptr;

Nedrysoft::MainWindow::MainWindow(Nedrysoft::SplashScreen *splashScreen)
        : QMainWindow(nullptr),
          ui(new Ui::MainWindow),
          m_minimumPixelArea(10000),
          m_backgroundImage(),
          m_grid(20,20),
          m_gridIsVisible(true),
          m_gridShouldSnap(true),
          m_snapToFeatures(true),
          m_showIcons(true) {

    ui->setupUi(this);

    qobject_cast<QApplication *>(QCoreApplication::instance())->installEventFilter(this);

    m_instance = this;

    m_builder = new Builder;

    QTimer::singleShot(splashScreenDuration, [splashScreen]() {
        splashScreen->close();
    });

    connect(ui->actionQuit, &QAction::triggered, [this](bool isChecked){
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

        if (ui->featureAutpDetectCheckbox->isChecked()) {
            processBackground();
        }
    });

    ui->gridVisibleCheckbox->setCheckState(m_gridIsVisible ? Qt::Checked : Qt::Unchecked);
    ui->gridSnapCheckbox->setCheckState(m_gridShouldSnap ? Qt::Checked : Qt::Unchecked);
    ui->featureAutpDetectCheckbox->setCheckState(m_snapToFeatures ? Qt::Checked : Qt::Unchecked);
    ui->showIconsCheckBox->setCheckState(m_showIcons ? Qt::Checked : Qt::Unchecked);

    connect(ui->gridVisibleCheckbox, &QCheckBox::stateChanged, [this](int state) {
        ui->previewWidget->setGrid(QSize(20,20), (state==Qt::Checked) ? true:false, true);
    });

    connect(ui->showIconsCheckBox, &QCheckBox::stateChanged, [this](int state) {
        ui->previewWidget->setIconsVisible((state==Qt::Checked) ? true:false);
    });

    connect(ui->featureAutpDetectCheckbox, &QCheckBox::stateChanged, [this](int state) {
        if (!state) {
            ui->previewWidget->clearCentroids();
        } else {
            processBackground();
        }
    });

    connect(ui->insertAppPushButton, &QPushButton::clicked, [](bool isChecked) {

    });

    connect(ui->dropAppPushButton, &QPushButton::clicked, [this](bool isChecked) {
        QMenu popupMenu;
        auto menuPos = ui->dropAppPushButton->mapToGlobal(ui->dropAppPushButton->rect().bottomLeft());

        popupMenu.addAction("Shortcut To Applications");
        popupMenu.addAction("Shortcut...");
        popupMenu.addAction("Icon...");

        popupMenu.exec(menuPos);
    });

    connect(ui->iconsSizeLineEdit, &QLineEdit::textChanged, [this](const QString &text) {
        bool ok = false;
        int size = text.toInt(&ok);

        if ((ok) && (size!=0)) {
            ui->previewWidget->setIconSize(text.toInt());
        }
    });

    loadConfiguration("./dmgee.dmgee");

    processBackground();

    ui->previewWidget->setGrid(QSize(20,20), true, true);

    ui->featureAutpDetectCheckbox->setCheckState(Qt::Checked);

    QTemporaryDir temporaryDir;

    if (temporaryDir.isValid()) {
        auto temporaryName = temporaryDir.path()+"/Applications";

        if (QFile::link("/Applications", temporaryName)) {
            auto applicationsShortcutImage = new Nedrysoft::Image(temporaryName, false, 160, 160);
            auto applicationIcon = new Nedrysoft::Image("/Users/adriancarpenter/Documents/Development/dmgee/bin/x86_64/Debug/dmgee.app", false, 160, 160);

            ui->previewWidget->addIcon(applicationsShortcutImage, QPoint(100,100), PreviewWidget::Shortcut);
            ui->previewWidget->addIcon(applicationIcon, QPoint(100,100), PreviewWidget::Icon);
        }
    }

    connect(ui->ribbonDropButton, &Nedrysoft::Ribbon::RibbonDropButton::clicked, [=](bool clicked) {
        // TODO: handle click
    });
}

Nedrysoft::MainWindow::~MainWindow() {
    delete ui;
}

Nedrysoft::MainWindow *Nedrysoft::MainWindow::getInstance() {
    return m_instance;
}

void Nedrysoft::MainWindow::handleOpenByUrl(const QUrl &url) {
    Q_UNUSED(url);
}

bool Nedrysoft::MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type()==QEvent::FileOpen) {
        auto fileOpenEvent = dynamic_cast<QFileOpenEvent *>(event);

        /*if (!fileOpenEvent->url().isEmpty()) {
            // TODO: launched via url scheme
        } else if (!fileOpenEvent->file().isEmpty()) {
            // TODO: launched via file association
        }*/

        return false;
    }

    return QObject::eventFilter(obj, event);
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

        cv::threshold(image, image, 225, 255, cv::THRESH_BINARY);

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

            if (area > m_minimumPixelArea) {
                m_centroids.append(centroid);
            }
        }

        ui->previewWidget->setCentroids(m_centroids);
    }
}

QVariant Nedrysoft::MainWindow::configValue(const QString& valueName, QVariant defaultValue) {
    if (m_config.contains(valueName)) {
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

        ui->featureAutpDetectCheckbox->setCheckState(configValue("detectFeatures", true).toBool() ? Qt::Checked : Qt::Unchecked);

        updatePixmap();
    }

    return true;
}

void Nedrysoft::MainWindow::updatePixmap() {
    QFileInfo fileInfo(m_builder->property("background").toString());

    if (!fileInfo.absoluteFilePath().isEmpty()) {
        m_backgroundImage = Nedrysoft::Image(fileInfo.absoluteFilePath(), true);

        m_backgroundPixmap = QPixmap::fromImage(m_backgroundImage.image());

        ui->previewWidget->setPixmap(m_backgroundPixmap);

        if (ui->featureAutpDetectCheckbox->isChecked()) {
            processBackground();
        }
    } else {
        m_backgroundPixmap = QPixmap();

        ui->previewWidget->setPixmap(m_backgroundPixmap);
        ui->previewWidget->clearCentroids();
    }
}
