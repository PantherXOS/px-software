//
// Created by hamzeh on 10/22/19.
//

#include "CategoryLayout.h"

CategoryLayout::CategoryLayout(PKG::Category *category) {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);

    name = category->name().toStdString();
    title = category->title().toStdString();
    description = category->description().toStdString();
    icon = ":/category/icons/" + category->icon().toStdString();
    QLabel *titleLabel= new QLabel();
    titleLabel->setText(QString(title.c_str()));
    titleLabel->setFont(titleFont);

    QLabel *descriptionLabel = new QLabel();
    descriptionLabel->setText(QString(description.c_str()));
    descriptionLabel->setFont(descriptionFont);

    QPushButton *iconButton = new QPushButton();
    QIcon qicon(icon.c_str());
    iconButton->setIcon(qicon);
    iconButton->setIconSize(QSize(64,64));
    iconButton->setFixedSize(QSize(64,64));

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(titleLabel);
    vLayout->addWidget(descriptionLabel);
    QWidget *qWidget = new QWidget();
    qWidget->setLayout(vLayout);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(iconButton);
    layout->addWidget(qWidget);

    this->setLayout(layout);
}

string CategoryLayout::getName() {
    return name;
}

string CategoryLayout::getTitle() {
    return title;
}

string CategoryLayout::getDescription() {
    return description;
}

string CategoryLayout::getIcon() {
    return icon;
}
