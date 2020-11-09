/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
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
 */

#include "PackageListWidgetItem.h"

PackageListWidgetItem::PackageListWidgetItem(Package *package, bool removeEnable ,QWidget *parent) : QWidget(parent) {
    packageComponent = new PackageComponent(package,removeEnable,this);
    connect(packageComponent, SIGNAL(showTerminalSignal(TerminalWidget *)),this, SLOT(showTerminalSignalHandler(TerminalWidget *)));

    setContentsMargins(10,5,10,5);
    this->package = package;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(packageComponent->getIconLayout());
    layout->addLayout(loadTexts());
    layout->addLayout(packageComponent->getButtonsLayoutAsList());
    this->setLayout(layout);
}

void PackageListWidgetItem::showTerminalSignalHandler(TerminalWidget *terminal){
    emit showTerminalSignal(terminal);
}

QVBoxLayout *PackageListWidgetItem::loadTexts() {
    QFont titleFont("default", PACKAGE_TITLE_FONT_SIZE,QFont::Bold);
    QFont descriptionFont("default", PACKAGE_DESC_FONT_SIZE);
    // add title, license and desc
    auto titleLabel= new QLabel(this->package->title(),this);
    titleLabel->setFont(titleFont);
    titleLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    auto titleLayout = new QHBoxLayout;
    titleLayout->setAlignment(Qt::AlignRight);
    titleLayout->addWidget(titleLabel);

    auto licenseLabel= new QLabel(this->package->version() + " - " + this->package->license(),this);
    licenseLabel->setStyleSheet(PACKAGE_LICENSE_STYLESHEET);
    auto licenseLayout = new QHBoxLayout;
    licenseLayout->setAlignment(Qt::AlignLeft);
    licenseLayout->addWidget(licenseLabel);

    auto descriptionLabel= new QLabel(this->package->description().mid(0,150).append(" ... more"),this);
    descriptionLabel->setFont(descriptionFont);
    // descriptionLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    descriptionLabel->setWordWrap(true);

    auto descriptionLayout = new QHBoxLayout;
    descriptionLayout->addWidget(descriptionLabel);

    auto up = new QHBoxLayout;
    up->setAlignment(Qt::AlignLeft);
    up->addLayout(titleLayout);
    up->addLayout(licenseLayout);

    auto down = new QHBoxLayout;
    down->addLayout(descriptionLayout);

    auto textLayout = new QVBoxLayout;
    textLayout->setAlignment(Qt::AlignTop);
    textLayout->addLayout(up);
    textLayout->addLayout(down);
    textLayout->setContentsMargins(20,0,20,0);
    return textLayout;
}

Package * &PackageListWidgetItem::getPackage() {
    return this->package;
}

TerminalWidget *PackageListWidgetItem::getTerminal() {
    return packageComponent->getTerminal();
}
