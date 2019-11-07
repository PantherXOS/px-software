//
// Created by hamzeh on 10/22/19.
//

#include "CategoryWidget.h"

CategoryWidget::CategoryWidget(PKG::Category *category) {
    QFont titleFont("default", 12,QFont::Bold);
    QFont descriptionFont("default", 10);

    name = category->name();
    title = category->title();
    description = category->description();

    // check url is weblink or name of resource file
    QRegExp rx("https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|www\\.[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\\.[^\\s]{2,}|https?:\\/\\/(?:www\\.|(?!www))[a-zA-Z0-9]+\\.[^\\s]{2,}|www\\.[a-zA-Z0-9]+\\.[^\\s]{2,}");
    if(rx.exactMatch(category->icon())){
        cout << "TBD - iconRemoteUrl is weblink url : " << category->icon().toStdString() << endl;
    }
    else
        icon = ":/category/icons/" + category->icon();
    QLabel *titleLabel= new QLabel();
    titleLabel->setText(title);
    titleLabel->setFont(titleFont);

    QLabel *descriptionLabel = new QLabel();
    descriptionLabel->setText(description);
    descriptionLabel->setFont(descriptionFont);

    QLabel *iconButton = new QLabel;
    QIcon qicon;
    QImage image(icon);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(QSize(64,64), QIcon::Normal, QIcon::On);
    iconButton->setPixmap(pixmap);
    iconButton->setFixedSize(QSize(64,64));

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(titleLabel);
    vLayout->addWidget(descriptionLabel);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(iconButton);
    layout->addLayout(vLayout);

    this->setLayout(layout);
}

//QString CategoryWidget::getName() {
//    return this->name;
//}
//
//QString CategoryWidget::getTitle() {
//    return this->title;
//}
//
//QString CategoryWidget::getDescription() {
//    return this->description;
//}
//
//QString CategoryWidget::getIcon() {
//    return this->iconRemoteUrl;
//}

PxQScrollArea * CategoryWidget::getPackageList() {
    PxQScrollArea *scrollArea = new PxQScrollArea(0,name);
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QString m_dbPath = "./SAMPLE_DB";
    PKG::DataAccessLayer dbLayer(m_dbPath);
    auto pkgs = dbLayer.categoryPackages(name);
    for(auto pkg:pkgs){
        PackageWidget *packageWidget = new PackageWidget(pkg);
//        packageWidget->showMaximized();
        boxLayout->addWidget(packageWidget);
    }
    QWidget *widget=new QWidget;
    widget->setLayout(boxLayout);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    scrollArea->showMaximized();
    return scrollArea;
}

