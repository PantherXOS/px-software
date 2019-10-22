//
// Created by hamzeh on 10/22/19.
//

#include "CategoryLayout.h"

CategoryLayout::CategoryLayout(){
    /// todo
    id = 0;
    title = "title";
    description = "description";
    icon = "icon";
    /// ---
    QLabel *titleLabel= new QLabel();
    titleLabel->setText(QString(title.c_str()));

    QLabel *descriptionLabel = new QLabel();
    descriptionLabel->setText(QString(description.c_str()));

    QLabel *iconLabel = new QLabel();
    iconLabel->setText(QString(icon.c_str()));
    iconLabel->setFixedSize(56,56);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(titleLabel);
    vLayout->addWidget(descriptionLabel);
    QWidget *qWidget = new QWidget();
    qWidget->setLayout(vLayout);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(iconLabel);
    layout->addWidget(qWidget);

    this->setLayout(layout);
}

int CategoryLayout::getId() {
    return id;
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
