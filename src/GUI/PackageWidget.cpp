//
// Created by hamzeh on 11/3/19.
//

#include "PackageWidget.h"

PackageWidget::PackageWidget(PKG::Package *package) {
    name = package->name();
    title = package->title();
    version = package->version();
    description = package->description();
    homepage = package->homePage();
    license = package->license();
    icon = package->icon();
    screenshotsList = package->screenShots();
    categoriesList = package->categories();

    QPushButton *iconButton = new QPushButton();
    QIcon qicon(icon);
    iconButton->setIcon(qicon);
    iconButton->setIconSize(QSize(64,64));
    iconButton->setFixedSize(QSize(64,64));

    // add title, license and desc
    QLabel *titleLabel= new QLabel(title);
    QLabel *licenseLabel= new QLabel(license);
    QLabel *descriptionLabel= new QLabel(description);
    QHBoxLayout *up = new QHBoxLayout;
    up->addWidget(titleLabel);
    up->addWidget(licenseLabel);

    QHBoxLayout *down = new QHBoxLayout;
    down->addWidget(descriptionLabel);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->addLayout(up);
    textLayout->addLayout(down);

    // add install,update and remove buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    if(0) { // if installed
        if (1) { // if updatable
            QPushButton *updateButton = new QPushButton;
            updateButton->setText("Update");
            buttonLayout->addWidget(updateButton);
        }
        QPushButton *removeButton = new QPushButton;
        removeButton->setText("Remove");
        buttonLayout->addWidget(removeButton);
    } else {
        QPushButton *installButton = new QPushButton;
        installButton->setText("Install");
        buttonLayout->addWidget(installButton);
    }

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(iconButton);
    layout->addLayout(textLayout);
    layout->addLayout(buttonLayout);

    this->setLayout(layout);
}
