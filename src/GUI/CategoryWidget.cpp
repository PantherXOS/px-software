//
// Created by hamzeh on 10/22/19.
//

#include "CategoryWidget.h"

CategoryWidget::CategoryWidget(PKG::Category *category) {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);

    name = category->name().toStdString();
    title = category->title().toStdString();
    description = category->description().toStdString();

    // check url is weblink or name of resource file
    QRegExp rx("https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|www\\.[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9]+\\.[^\\s]{2,}|www\\.[a-zA-Z0-9]+\\.[^\\s]{2,}");
    if(rx.exactMatch(category->icon())){
        cout << "TBD - icon is weblink url : " << category->icon().toStdString() << endl;
    }
    else
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

string CategoryWidget::getName() {
    return name;
}

string CategoryWidget::getTitle() {
    return title;
}

string CategoryWidget::getDescription() {
    return description;
}

string CategoryWidget::getIcon() {
    return icon;
}
