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
#include <QWindow>
#include <QTimer>

using namespace std::chrono_literals;

constexpr auto splashScreenDuration = 3s;

Nedrysoft::MainWindow *Nedrysoft::MainWindow::m_instance = nullptr;

Nedrysoft::MainWindow::MainWindow(Nedrysoft::SplashScreen *splashScreen)
        : QMainWindow(nullptr),
          ui(new Ui::MainWindow) {

    ui->setupUi(this);

    qApp->installEventFilter(this);

    m_instance = this;

    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point2f> centroids;
    cv::Mat image;// = m_backgroundImage->mat();

    // convert the image to grey scale for contour detection

    //cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    /*showMaximized();

    ui->webEngineView->setPage(m_page);
*/
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

    unsigned int loadedImageLength;
    char *loadedImageData;

    auto filename = QString("/Users/adriancarpenter/Documents/Development/dmgee/assets/splash_620x375@2x.png");

    //auto loadedImage = QImage();

    auto loadedImage = new Nedrysoft::Image(filename, true);

    m_backgroundPixmap = QPixmap::fromImage(loadedImage->image());

    ui->previewWidget->setPixmap(m_backgroundPixmap);

    QDesktopServices::setUrlHandler("dmgee", this, SLOT("handleOpenByUrl"));
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

/*
void Nedrysoft::Application::processThread()
{
    std::unique_lock lock(m_processMutex);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point2f> centroids;
    cv::Mat image = m_backgroundImage->mat();

    // convert the image to grey scale for contour detection

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    while(!m_stopProcessThread.load()) {
        centroids.clear();
        hierarchy.clear();
        contours.clear();

        // apply thresholding

        cv::threshold(image, image, 150, 255, cv::THRESH_BINARY);

        // find contours in image

        cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        // find centre of discovered objects in image

        for (auto & contour : contours) {
            float sumX = 0, sumY = 0;
            float size = contour.size();
            cv::Point2f centroid;

            if(size > 0) {
                for (auto & point : contour) {
                    sumX += point.x;
                    sumY += point.y;
                }

                centroid.x = sumX/size;
                centroid.y = sumY/size;
            }

            auto area = cv::contourArea(contour);

            if (area>m_minArea.load()) {
                centroids.push_back(centroid);
            }
        }

        std::unique_lock centroidLock(m_centroidsMutex);

        m_centroids = centroids;

        centroidLock.unlock();

        m_processCondition.wait(lock);
    }
}
 */
