/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 18/11/2020.
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

#include "HTermWidget.h"

#include "HTermApi.h"

#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

Nedrysoft::HTermWidget::HTermWidget(QWidget *parent) :
        m_terminalReady(false) {

    m_webEngineView = new QWebEngineView;

    m_layout = new QGridLayout;

    m_layout->addWidget(m_webEngineView);

    m_layout->setMargin(0);

    setLayout(m_layout);

    setContentsMargins(0,0,0,0);

    m_profile = new QWebEngineProfile;

    m_profile->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);

    m_page = new QWebEnginePage(m_profile);

    m_page->setBackgroundColor(Qt::black);

    m_apiChannel = new QWebChannel;
    m_terminalApi = new HTermApi;

    m_apiChannel->registerObject("terminalApi", m_terminalApi);

    m_page->setWebChannel(m_apiChannel);

    m_webEngineView->setPage(m_page);

    m_page->load(QUrl("qrc:/hterm/index.html"));

    connect(m_terminalApi, &Nedrysoft::HTermApi::terminalReady, [=]() {
        m_terminalReady = true;

        Q_EMIT terminalReady();
    });
}

Nedrysoft::HTermWidget::~HTermWidget() {
    delete m_page;
    delete m_webEngineView;
    delete m_profile;

    delete m_layout;
    delete m_apiChannel;
    delete m_terminalApi;
}

void Nedrysoft::HTermWidget::print(QString string) {
    if (m_terminalReady) {
        m_terminalApi->print(string);
    }
}

void Nedrysoft::HTermWidget::println(QString string) {
    if (m_terminalReady) {
        m_terminalApi->println(string);
    }
}

bool Nedrysoft::HTermWidget::isReady() {
    return m_terminalReady;
}
