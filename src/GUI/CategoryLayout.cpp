//
// Created by hamzeh on 10/22/19.
//

#include "CategoryLayout.h"

CategoryLayout::CategoryLayout(PKG::Category *category) {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);

    name = category->name();
    title = category->title();
    description = category->description();

    // check url is weblink or name of resource file
    QRegExp rx("https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|www\\.[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9]+\\.[^\\s]{2,}|www\\.[a-zA-Z0-9]+\\.[^\\s]{2,}");
    if(rx.exactMatch(category->icon())){
        cout << "TBD - icon is weblink url : " << category->icon().toStdString() << endl;
    }
    else
        icon = ":/category/icons/" + category->icon();
    QLabel *titleLabel= new QLabel();
    titleLabel->setText(title);
    titleLabel->setFont(titleFont);

    QLabel *descriptionLabel = new QLabel();
    descriptionLabel->setText(description);
    descriptionLabel->setFont(descriptionFont);

    QPushButton *iconButton = new QPushButton();
    QIcon qicon(icon);
    iconButton->setIcon(qicon);
    iconButton->setIconSize(QSize(64,64));
    iconButton->setFixedSize(QSize(64,64));
    iconButton->setText(name);
    connect(iconButton, SIGNAL (released()), SLOT (handleCategoryButton()));

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

void CategoryLayout::handleCategoryButton() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        cout << button->text().toStdString() << endl;
    }
}