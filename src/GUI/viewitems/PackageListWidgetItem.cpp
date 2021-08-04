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

PackageListWidgetItem_widget::PackageListWidgetItem_widget(Package *package, bool updateEnable, bool removeEnable ,QWidget *parent) : QWidget(parent) {
    _packageComponent = new PackageComponent(package,updateEnable,removeEnable,this);
    connect(_packageComponent, SIGNAL(showTerminalSignal(TerminalWidget *)),this, SLOT(showTerminalSignalHandler(TerminalWidget *)));
    setContentsMargins(10,0,10,0);
    this->package = package;
    QHBoxLayout *layout = new QHBoxLayout;
    if(package->isAvailableInDB()) 
        layout->addLayout(_packageComponent->getIconLayout());
    layout->addLayout(loadTexts());
    layout->addLayout(_packageComponent->getButtonsLayoutAsList());
    setLayout(layout);
    auto pal = QGuiApplication::palette();
    auto bgcolor =  pal.color(QPalette::Normal, QPalette::Highlight);
    setStyleSheet( QString::fromLatin1(ITEM_HOVER_STYLESHEET).arg(bgcolor.name()));
}

void PackageListWidgetItem_widget::showTerminalSignalHandler(TerminalWidget *terminal){
    emit showTerminalSignal(terminal);
}

QVBoxLayout *PackageListWidgetItem_widget::loadTexts() {
    QFont titleFont("default", PACKAGE_TITLE_FONT_SIZE,QFont::Bold);
    QFont descriptionFont("default", PACKAGE_DESC_FONT_SIZE);
    // add title, license and desc
    auto titleLabel= new QLabel(this->package->title(),this);
    titleLabel->setFont(titleFont);
    titleLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    titleLabel->setStyleSheet(PACKAGE_LIST_LABELS_STYLESHEET);
    
    auto titleLayout = new QHBoxLayout;
    titleLayout->setAlignment(Qt::AlignRight);
    titleLayout->addWidget(titleLabel);

    auto up = new QHBoxLayout;
    up->setAlignment(Qt::AlignLeft);
    up->addLayout(titleLayout);
    
    auto textLayout = new QVBoxLayout;
    textLayout->setAlignment(Qt::AlignRight |  Qt::AlignVCenter);
    textLayout->addLayout(up);
    textLayout->setContentsMargins(20,0,20,0);
    
    if(!this->package->version().isEmpty() && !this->package->license().isEmpty()) {
        auto licenseLabel= new QLabel(this->package->version() + " - " + this->package->license(),this);
        licenseLabel->setStyleSheet(PACKAGE_LICENSE_STYLESHEET);
        auto licenseLayout = new QHBoxLayout;
        licenseLayout->setAlignment(Qt::AlignLeft);
        licenseLayout->addWidget(licenseLabel);
        up->addLayout(licenseLayout);
    }

    if(!this->package->description().isEmpty()){
        auto down = new QHBoxLayout;
        auto descriptionLabel= new QLabel(this->package->description().mid(0,150).append(" ... more"),this);
        descriptionLabel->setStyleSheet(PACKAGE_LIST_LABELS_STYLESHEET);
        descriptionLabel->setFont(descriptionFont);
        // descriptionLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        descriptionLabel->setWordWrap(true);

        auto descriptionLayout = new QHBoxLayout;
        descriptionLayout->addWidget(descriptionLabel);
        down->addLayout(descriptionLayout);    
        textLayout->addLayout(down);
    }
    return textLayout;
}

Package * &PackageListWidgetItem_widget::getPackage() {
    return this->package;
}

TerminalWidget *PackageListWidgetItem_widget::getTerminal() {
    return _packageComponent->getTerminal();
}

void PackageListWidgetItem_widget::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PackageListWidgetItem_widget::enableUpdateAllButton() {
    _packageComponent->enableUpdateAllButton();
}
    
PackageComponent *PackageListWidgetItem_widget::packageComponent(){
    return _packageComponent;
}