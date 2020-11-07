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
#include <QDebug>
#include <QDesktopServices>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QWindow>

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
          m_gridShouldSnap(true) {

    ui->setupUi(this);

    qApp->installEventFilter(this);

    m_instance = this;

    QTimer::singleShot(splashScreenDuration, [splashScreen]() {
        splashScreen->close();
    });

    connect(ui->actionQuit, &QAction::triggered, [this](bool isChecked){
        close();
    });

    connect(ui->actionAbout, &QAction::triggered, [this](bool isChecked) {
        Nedrysoft::AboutDialog aboutDialog;

        aboutDialog.exec();
    });

    auto filename = QString("/Users/adriancarpenter/Documents/Development/dmgee/assets/dmg_background@2x.tiff");

    m_backgroundImage = Nedrysoft::Image(filename, true);

    m_backgroundPixmap = QPixmap::fromImage(m_backgroundImage.image());

    ui->previewWidget->setPixmap(m_backgroundPixmap);

    QDesktopServices::setUrlHandler("dmgee", this, SLOT("handleOpenByUrl"));

    processBackground();

    connect(ui->minFeatureSlider, &QSlider::valueChanged, [this](int newValue) {
        m_minimumPixelArea = newValue;
        processBackground();
    });

    ui->gridVisibleCheckbox->setCheckState(m_gridIsVisible?Qt::Checked:Qt::Unchecked);
    ui->snapGridCheckbox->setCheckState(m_gridShouldSnap?Qt::Checked:Qt::Unchecked);

    connect(ui->gridVisibleCheckbox, &QCheckBox::stateChanged, [this](int state) {
        ui->previewWidget->setGrid(QSize(20,20), (state==Qt::Checked) ? true:false, true);
    });

    ui->previewWidget->setGrid(QSize(20,20), true, true);
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
        QFileOpenEvent *fileOpenEvent = static_cast<QFileOpenEvent*>(event);

        if (!fileOpenEvent->url().isEmpty()) {
            // fileOpenEvent->url() contains the url if launched via url scheme
        } else if (!fileOpenEvent->file().isEmpty()) {
            // fileOpenEvent->file() contains the filename if launched via file association
        }

        return false;
    }

    return QObject::eventFilter(obj, event);
}

void Nedrysoft::MainWindow::closeEvent(QCloseEvent *closeEvent) {
    closeEvent->accept();
}

void Nedrysoft::MainWindow::processBackground()
{
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat image = m_backgroundImage.mat();

    // convert the image to grey scale for contour detection

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    m_centroids.clear();

    // apply thresholding

    cv::threshold(image, image, 150, 255, cv::THRESH_BINARY);

    // find contours in image

    cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // find centre of discovered objects in image

    for (auto & contour : contours) {
        float sumX = 0, sumY = 0;
        float size = contour.size();
        QPointF centroid;

        if(size > 0) {
            for (auto & point : contour) {
                sumX += point.x;
                sumY += point.y;
            }

            centroid = QPointF(sumX/size, sumY/size);
        }

        auto area = cv::contourArea(contour);

        if (area>m_minimumPixelArea) {
            m_centroids.append(centroid);
        }
    }

    ui->previewWidget->setCentroids(m_centroids);
}
