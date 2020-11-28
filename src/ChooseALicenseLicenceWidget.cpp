/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of dmgee
 *
 * Created by Adrian Carpenter on 27/11/2020.
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

#include "ChooseALicenseLicenceWidget.h"

#include "ui_ChooseALicenseLicenceWidget.h"

#include <BulletWidget.h>
#include <QDebug>
#include <QFile>

Nedrysoft::ChooseALicenseLicenceWidget::ChooseALicenseLicenceWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ChooseALicenseLicenceWidget),
        m_licence(nullptr) {

    ui->setupUi(this);

    loadRules(":/choosealicence.com/_data/rules.yml");

    ui->titleLabel->setText("");
    ui->descriptionLabel->setText("");
    ui->licenseText->setText("");
    ui->licenseText->setFont(QFont("Fira Code", ui->licenseText->font().pointSize()));

    ui->titleLine ->setVisible(false);
    ui->licenceLine ->setVisible(false);
    ui->attributesLine->setVisible(false);
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
}

Nedrysoft::ChooseALicenseLicenceWidget::ChooseALicenseLicenceWidget(Nedrysoft::ChooseALicenseLicence *licence) :
        ChooseALicenseLicenceWidget(static_cast<QWidget *>(nullptr)) {

    m_licence = licence;

    updateContent();
}

void Nedrysoft::ChooseALicenseLicenceWidget::createAttributeWidgets(Type type) {
    QVBoxLayout *layout;
    QMap<QString, Rule> rules;
    QString colour;
    QStringList conditions;
    QString title;

    if (type==Conditions) {
        layout = m_conditionsLayout;
        rules = m_conditionsRules;
        conditions = m_licence->conditions();
        title = "Conditions";
        colour = "blue";
    } else if (type==Limitations) {
        layout = m_limitationsLayout;
        rules = m_limitationsRules;
        conditions = m_licence->limitations();
        title = "Limitations";
        colour = "red";
    } else {
        layout = m_permissionsLayout;
        rules = m_permissionsRules;
        conditions = m_licence->permissions();
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

        bulletWidget->setToolTip(rules[string].description());

        layout->addWidget(bulletWidget);
    }
}

bool Nedrysoft::ChooseALicenseLicenceWidget::loadRules(QString filename) {
    QFile rulesFile(filename);

    m_permissionsRules.clear();
    m_limitationsRules.clear();
    m_conditionsRules.clear();

    if (rulesFile.open(QFile::ReadOnly)) {
        YAML::Node rulesDocument = YAML::Load(rulesFile.readAll().toStdString());

        if (!rulesDocument.IsNull()) {
            m_permissionsRules = importRule(rulesDocument, Permissions);
            m_limitationsRules = importRule(rulesDocument, Limitations);
            m_conditionsRules = importRule(rulesDocument, Conditions);

            return true;
        }
    }

    return false;
}

QMap<QString, Nedrysoft::ChooseALicenseLicenceWidget::Rule> Nedrysoft::ChooseALicenseLicenceWidget::importRule(YAML::Node rulesDocument, Type type) {
    QMap<QString, Rule> rules;
    QString typeString;

    if (type == Conditions) {
        typeString = "conditions";
    } else if (type == Limitations) {
        typeString = "limitations";
    } else {
        typeString = "permissions";
    }

    if (rulesDocument[typeString.toStdString()].IsSequence()) {
        auto sequence = rulesDocument[typeString.toStdString()];

        for (auto map : sequence) {
            if (map.IsMap()) {
                Rule newRule;

                newRule.setDescription(QString::fromStdString(map["description"].as<std::string>()));
                newRule.setTag(QString::fromStdString(map["tag"].as<std::string>()));
                newRule.setLabel(QString::fromStdString(map["label"].as<std::string>()));

                rules[newRule.tag()] = newRule;
            }
        }
    }

    return rules;
}

void Nedrysoft::ChooseALicenseLicenceWidget::updateContent(void) {
    ui->licenceLine->setVisible(m_licence->valid());
    ui->titleLine ->setVisible(m_licence->valid());
    ui->attributesLine->setVisible(m_licence->valid());
    ui->descriptionLabel->setVisible(m_licence->valid());
    ui->titleLabel->setVisible(m_licence->valid());

    if (!m_licence->valid()) {
        return;
    }

    ui->titleLabel->setText(m_licence->title());
    ui->descriptionLabel->setText(m_licence->description());
    ui->licenseText->setText(m_licence->licenceText());

    createAttributeWidgets(Permissions);
    createAttributeWidgets(Conditions);
    createAttributeWidgets(Limitations);

    m_limitationsLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    m_conditionsLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    m_permissionsLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
}