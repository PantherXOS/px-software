//
// Created by hamzeh on 11/3/19.
//

#include "PackageListWidgetItem.h"

PackageListWidgetItem::PackageListWidgetItem(Package *package, bool removeEnable ,QWidget *parent) : QWidget(parent) {
    packageComponent = new PackageComponent(package,removeEnable,this);

    this->package = package;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(packageComponent->getIconLayout());
    layout->addLayout(loadTexts());
    layout->addLayout(packageComponent->getButtonsLayoutAsList());
    this->setLayout(layout);
}

QVBoxLayout *PackageListWidgetItem::loadTexts() {
    QFont titleFont("default", PACKAGE_TITLE_FONT_SIZE,QFont::Bold);
    QFont descriptionFont("default", PACKAGE_DESC_FONT_SIZE);
    // add title, license and desc
    QLabel *titleLabel= new QLabel(this->package->title(),this);
    titleLabel->setFont(titleFont);

    QLabel *licenseLabel= new QLabel(this->package->version() + " - " + this->package->license(),this);
    licenseLabel->setStyleSheet("QLabel { color : gray; }");

    QLabel *descriptionLabel= new QLabel(this->package->description().mid(0,300).append(" ... more"),this);
    descriptionLabel->setFont(descriptionFont);
    descriptionLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    descriptionLabel->setWordWrap(true);

    auto descriptionLayout = new QHBoxLayout;
    descriptionLayout->addWidget(descriptionLabel);

    auto up = new QHBoxLayout;
    up->addWidget(titleLabel);
    up->addWidget(licenseLabel);

    auto down = new QHBoxLayout;
    down->addLayout(descriptionLayout);

    auto textLayout = new QVBoxLayout;
    textLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    textLayout->addLayout(up);
    textLayout->addLayout(down);
    return textLayout;
}

Package * &PackageListWidgetItem::getPackage() {
    return this->package;
}

TerminalWidget *PackageListWidgetItem::getTerminal() {
    return packageComponent->getTerminal();
}
