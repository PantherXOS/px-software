//
// Created by hamzeh on 12/31/19.
//

#ifndef PX_SOFTWARE_PXSIDEBARITEM_H
#define PX_SOFTWARE_PXSIDEBARITEM_H
#include <QListWidgetItem>
#include <QLabel>
#include <QHBoxLayout>
#include <QGuiApplication>

#include "PXScrollArea.h"
#include "PXSeperator.h"
#include "PXParamSettings.h"

class PXSideBarItem : public QListWidgetItem{
public:
    enum ItemType{
        Item=0,
        Subitem=1
    };

    PXSideBarItem(const QString &title, ItemType itemType, PXScrollArea *view, QListWidget *parent = nullptr) : QListWidgetItem (parent){
        this->view = view;
        _title=title;
        titleLabel = new QLabel(title);
        if(itemType == ItemType::Item){
            QFont font("default", SIDEBAR_ITEM_FONT_SIZE, QFont::Bold);
            titleLabel->setFont(font);
        } else {
            QFont font("default", SIDEBAR_SUBITEM_FONT_SIZE);
            titleLabel->setFont(font);
        }
        auto pal = QGuiApplication::palette();
        auto fgcolor =  pal.color(QPalette::Active, QPalette::WindowText);
        QString sheet = QString::fromLatin1(SIDEBAR_ITEM_LABE_STYLE).arg(fgcolor.name());
        titleLabel->setStyleSheet(sheet);

        rightLayout = new QHBoxLayout;
        rightLayout->setAlignment(Qt::AlignRight);

        icon = new QLabel;
        icon->setHidden(true);
 
        auto leftLayout = new QHBoxLayout;
        leftLayout->addWidget(icon);
        leftLayout->addWidget(titleLabel);
        leftLayout->setAlignment(Qt::AlignLeft);

        auto layout = new QHBoxLayout;
        layout->addLayout(leftLayout);
        layout->addLayout(rightLayout);

        auto seperator = new PXSeperator;

        auto mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(SIDEBAR_ITEM_MARGIN_LEFT,SIDEBAR_ITEM_MARGIN_TOP,SIDEBAR_ITEM_MARGIN_RIGHT,SIDEBAR_ITEM_MARGIN_BOTTOM);
        if(itemType == ItemType::Item){
            mainLayout->addLayout(layout);
            mainLayout->setAlignment(Qt::AlignBottom);
        } else {
            mainLayout->addLayout(layout);
            mainLayout->addWidget(seperator);
//            mainLayout->setAlignment(Qt::AlignTop);
        }

        customWidget = new QWidget;
        customWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        customWidget->setLayout(mainLayout);
    }

    void setIcon(const QString &file, const QSize &size = QSize(SIDEBAR_ITEM_ICON_SIZE, SIDEBAR_ITEM_ICON_SIZE)){
        QIcon qicon;
        QImage image(file);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
        icon->setPixmap(pixmap);
        icon->setVisible(true);
    }

    void setIcon(const QIcon &_icon, const QSize &size= QSize(SIDEBAR_ITEM_ICON_SIZE, SIDEBAR_ITEM_ICON_SIZE)){
        QPixmap pixmap = _icon.pixmap(size, QIcon::Normal, QIcon::On);
        icon->setPixmap(pixmap);
        icon->setVisible(true);
    }

    PXScrollArea *getView(){
        return view;
    }

    QWidget *getCustomWidget(){
        return customWidget;
    }

    void addWidget(QWidget *widget){
        rightLayout->addWidget(widget);
    }

    void addLayout(QLayout *layout) {
        rightLayout->addLayout(layout);
    }

    void setFont(const QFont &font){
        titleLabel->setFont(font);
    }

    QString title(){
        return _title;
    }

private:
    QString _title;
    QLabel *icon;
    PXScrollArea *view = nullptr;
    QWidget *customWidget;
    QHBoxLayout *rightLayout;
    QLabel *titleLabel;
};


#endif //PX_SOFTWARE_PXSIDEBARITEM_H
