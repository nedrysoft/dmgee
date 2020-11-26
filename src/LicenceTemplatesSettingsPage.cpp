/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 24/11/2020.
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

#include "LicenceTemplatesSettingsPage.h"

#include "BulletWidget.h"
#include "MainWindow.h"
#include "ui_LicenceTemplatesSettingsPage.h"

#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QModelIndex>
#include <QUrl>

Nedrysoft::LicenceTemplatesSettingsPage::LicenceTemplatesSettingsPage(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LicenceTemplatesSettingsPage) {

    ui->setupUi(this);

    loadRules(":/licences/rules.yaml");

    QDirIterator it(":/licences", QDirIterator::Subdirectories);

    QStandardItem *builtInLicenses = new QStandardItem(tr("Built In"));

    builtInLicenses->setEditable(false);

    while (it.hasNext()) {
        auto filename = it.next();
        auto licence = loadLicence(filename);

        if (licence.m_valid) {
            auto item = new QStandardItem;

            item->setText(licence.m_spdxId);
            item->setData(QVariant::fromValue<Nedrysoft::LicenceTemplatesSettingsPage::Licence>(licence), Qt::UserRole);
            item->setEditable(false);

            builtInLicenses->appendRow(item);
        }
    }

    m_licenceModel.appendRow(builtInLicenses);

    ui->outlineView->setItemModel(&m_licenceModel);

    connect(ui->outlineView, &ThemedOutlineView::buttonClicked, [=](int buttonIndex) {
        // TODO: handle the buttons
    });

    connect(ui->licenseText, &QTextBrowser::anchorClicked, [=](const QUrl &link) {
        // TODO: figure out why this isn't firing.
    });

    connect(ui->outlineView, &ThemedOutlineView::clicked, [=](const QModelIndex &index) {
        bool isLicense = false;

        if (index.parent().isValid()) {
            isLicense = true;
        }

        ui->line->setVisible(isLicense);
        ui->line_2->setVisible(isLicense);
        ui->line_3->setVisible(isLicense);
        ui->descriptionLabel->setVisible(isLicense);
        ui->titleLabel->setVisible(isLicense);

        auto licence = index.data(Qt::UserRole).value<Nedrysoft::LicenceTemplatesSettingsPage::Licence>();

        if ((!isLicense) || (!index.isValid())) {
            return;
        }

        ui->titleLabel->setText(licence.m_title);
        ui->descriptionLabel->setText(licence.m_description);
        ui->licenseText->setHtml(licence.m_licence);

        createList(Nedrysoft::LicenceTemplatesSettingsPage::Permissions, licence);
        createList(Nedrysoft::LicenceTemplatesSettingsPage::Conditions, licence);
        createList(Nedrysoft::LicenceTemplatesSettingsPage::Limitations, licence);

        m_limitationsLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
        m_conditionsLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
        m_permissionsLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    });

    ui->titleLabel->setText("");
    ui->descriptionLabel->setText("");
    ui->licenseText->setText("");

    ui->line->setVisible(false);
    ui->line_2->setVisible(false);
    ui->line_3->setVisible(false);
    ui->descriptionLabel->setVisible(false);
    ui->titleLabel->setVisible(false);

    m_mainLayout =  qobject_cast<QHBoxLayout *>(ui->attributesLayout->layout());

    m_permissionsLayout = new QVBoxLayout;
    m_conditionsLayout = new QVBoxLayout;
    m_limitationsLayout = new QVBoxLayout;

    m_limitationsLayout->setSpacing(2);
    m_permissionsLayout->setSpacing(2);
    m_conditionsLayout->setSpacing(2);

    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);

    m_mainLayout->addLayout(m_permissionsLayout);
    m_mainLayout->addLayout(m_conditionsLayout);
    m_mainLayout->addLayout(m_limitationsLayout);

#if defined(Q_OS_MACOS)
    m_size = QSize(qMax(minimumSizeHint().width(), size().width()), qMax(minimumSizeHint().height(), size().height()));
#else
    m_size = minimumSizeHint();
#endif
}

Nedrysoft::LicenceTemplatesSettingsPage::~LicenceTemplatesSettingsPage()
{
    delete ui;
}

bool Nedrysoft::LicenceTemplatesSettingsPage::canAcceptSettings()
{
    return true;
}

void Nedrysoft::LicenceTemplatesSettingsPage::acceptSettings()
{
}

QSize Nedrysoft::LicenceTemplatesSettingsPage::sizeHint() const
{
    return m_size;
}

Nedrysoft::LicenceTemplatesSettingsPage::Licence Nedrysoft::LicenceTemplatesSettingsPage::loadLicence(QString filename) {
    Nedrysoft::LicenceTemplatesSettingsPage::Licence licence;
    QFile licenseFile(filename);

    licence.m_valid = false;

    if (licenseFile.open(QFile::ReadOnly)) {
        QTextStream inputStream(licenseFile.readAll());

        if (inputStream.readLine() == "---") {
            QString yamlDocument;

            while (!inputStream.atEnd()) {
                auto currentLine = inputStream.readLine();

                if (currentLine == "---") {
                    break;
                }

                yamlDocument.append(currentLine + "\r\n");
            }

            YAML::Node header = YAML::Load(yamlDocument.toStdString());

            if (header.IsNull()) {
                return licence;
            }

            licence.m_licence = inputStream.readAll();

            if (header["title"].IsScalar()) {
                licence.m_title = QString::fromStdString(header["title"].as<std::string>());
            }

            if (header["spdx-id"].IsScalar()) {
                licence.m_spdxId = QString::fromStdString(header["spdx-id"].as<std::string>());
            }

            if (header["description"].IsScalar()) {
                licence.m_description = QString::fromStdString(header["description"].as<std::string>());
            }

            if (header["how"].IsScalar()) {
                licence.m_how = QString::fromStdString(header["how"].as<std::string>());
            }

            if (header["permissions"].IsSequence()) {
                auto stdSequence = header["permissions"].as<std::list<std::string>>();

                for (auto permission : stdSequence) {
                    licence.m_permissions.push_back(QString::fromStdString(permission));
                }
            }

            if (header["conditions"].IsSequence()) {
                auto stdSequence = header["conditions"].as<std::list<std::string>>();

                for (auto condition : stdSequence) {
                    licence.m_conditions.push_back(QString::fromStdString(condition));
                }
            }

            if (header["limitations"].IsSequence()) {
                auto stdSequence = header["limitations"].as<std::list<std::string>>();

                for (auto limitation : stdSequence) {
                    licence.m_limitations.push_back(QString::fromStdString(limitation));
                }
            }

            if (header["using"].IsMap()) {
                auto stdMap = header["using"].as<std::map<std::string, std::string>>();

                for (auto project : stdMap) {
                    licence.m_using[QString::fromStdString(project.first)] = QString::fromStdString(project.second);
                }
            }

            if (header["featured"].IsScalar()) {
                licence.m_featured = QVariant::fromValue<QString>(QString::fromStdString(header["featured"].as<std::string>())).toBool();
            }

            if (header["hidden"].IsScalar()) {
                licence.m_hidden = QVariant::fromValue<QString>(
                        QString::fromStdString(header["hidden"].as<std::string>())).toBool();
            }

            if (header["nickname"].IsScalar()) {
                licence.m_nickname = QString::fromStdString(header["nickname"].as<std::string>());
            }

            if (header["note"].IsScalar()) {
                licence.m_note = QString::fromStdString(header["note"].as<std::string>());
            }

            if (header["redirectfrom"].IsScalar()) {
                licence.m_redirectFrom = QString::fromStdString(header["redirectfrom"].as<std::string>());
            }

            licence.m_valid = true;
        }
    }

    return licence;
}

void Nedrysoft::LicenceTemplatesSettingsPage::createList(Nedrysoft::LicenceTemplatesSettingsPage::Type type, Nedrysoft::LicenceTemplatesSettingsPage::Licence licence) {
    QVBoxLayout *layout;
    QMap<QString, Rule> rules;
    QString colour;
    QStringList conditions;
    QString title;

    if (type==Nedrysoft::LicenceTemplatesSettingsPage::Conditions) {
        layout = m_conditionsLayout;
        rules = m_conditionsRules;
        conditions = licence.m_conditions;
        title = "Conditions";
        colour = "blue";
    } else if (type==Nedrysoft::LicenceTemplatesSettingsPage::Limitations) {
        layout = m_limitationsLayout;
        rules = m_limitationsRules;
        conditions = licence.m_limitations;
        title = "Limitations";
        colour = "red";
    } else {
        layout = m_permissionsLayout;
        rules = m_permissionsRules;
        conditions = licence.m_permissions;
        title = "Permissions";
        colour = "green";
    }

    while (1) {
        auto item = layout->takeAt(0);

        if (!item) {
            break;
        }

        if (item->widget()) {
            delete item->widget();
        }

        delete item;
    }

    auto label = new QLabel("<b>"+title+"</b>");

    layout->addWidget(label);

    layout->addSpacerItem(new QSpacerItem(1, 6));

    QStringList stringList;

    for (auto string: conditions) {
        auto bulletWidget = new BulletWidget(string, QPixmap(QString(":/icons/%1-dot@2x.png").arg(colour)));

        bulletWidget->setToolTip(rules[string].m_description);

        layout->addWidget(bulletWidget);
    }
}

bool Nedrysoft::LicenceTemplatesSettingsPage::loadRules(QString filename) {
    QFile rulesFile(filename);

    m_permissionsRules.clear();
    m_limitationsRules.clear();
    m_conditionsRules.clear();

    if (rulesFile.open(QFile::ReadOnly)) {
        YAML::Node rulesDocument = YAML::Load(rulesFile.readAll().toStdString());

        if (!rulesDocument.IsNull()) {
            m_permissionsRules = importRule(rulesDocument, Nedrysoft::LicenceTemplatesSettingsPage::Permissions);
            m_limitationsRules = importRule(rulesDocument, Nedrysoft::LicenceTemplatesSettingsPage::Limitations);
            m_conditionsRules = importRule(rulesDocument, Nedrysoft::LicenceTemplatesSettingsPage::Conditions);

            return true;
        }
    }

    return false;
}

QMap<QString, Nedrysoft::LicenceTemplatesSettingsPage::Rule> Nedrysoft::LicenceTemplatesSettingsPage::importRule(YAML::Node rulesDocument, Nedrysoft::LicenceTemplatesSettingsPage::Type type) {
    QMap<QString, Nedrysoft::LicenceTemplatesSettingsPage::Rule> rules;
    QString typeString;

    if (type == Nedrysoft::LicenceTemplatesSettingsPage::Conditions) {
        typeString = "conditions";
    } else if (type == Nedrysoft::LicenceTemplatesSettingsPage::Limitations) {
        typeString = "limitations";
    } else {
        typeString = "permissions";
    }

    if (rulesDocument[typeString.toStdString()].IsSequence()) {
        auto sequence = rulesDocument[typeString.toStdString()];

        for (auto map : sequence) {
            if (map.IsMap()) {
                Nedrysoft::LicenceTemplatesSettingsPage::Rule newRule;

                newRule.m_description = QString::fromStdString(map["description"].as<std::string>());
                newRule.m_tag = QString::fromStdString(map["tag"].as<std::string>());
                newRule.m_label = QString::fromStdString(map["label"].as<std::string>());

                rules[newRule.m_tag] = newRule;
            }
        }
    }

    return rules;
}