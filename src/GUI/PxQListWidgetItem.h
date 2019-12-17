//
// Created by hamzeh on 10/25/19.
//

#ifndef PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
#define PX_SETTINGS_UI_PXQLISTWIDGETITEM_H

#include <string>
#include <QObject>
#include <QWidget>
#include <QListWidgetItem>
#include <QFont>
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>

#include "Settings.h"

using namespace std;
class PxQListWidgetItem : public QListWidgetItem
{
public:
    PxQListWidgetItem(int id, const QString &title, const QFont &font, const QString &iconItemFile, QListWidget *parent = nullptr) : QListWidgetItem(parent){
        this->id = id;

        auto titleLabel = new QLabel(title);
        titleLabel->setFont(font);

        QIcon qicon;
        QImage image(iconItemFile);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        QPixmap pixmap = qicon.pixmap(QSize(CONTENT_LIST_ICON_SIZE, CONTENT_LIST_ICON_SIZE), QIcon::Normal, QIcon::On);
        auto iconItem = new QLabel;
        iconItem->setPixmap(pixmap);

        auto llayout = new QHBoxLayout;
        llayout->addWidget(iconItem);
        llayout->addWidget(titleLabel);
        llayout->setAlignment(Qt::AlignLeft);

        numberLabel = new QLabel();
        numberLabel->hide();
        rightIconLabel = new QLabel;

        auto rlayout = new QHBoxLayout;
        rlayout->addWidget(numberLabel);
        rlayout->addWidget(rightIconLabel);
        rlayout->setAlignment(Qt::AlignRight);

        auto mainLayout = new QHBoxLayout;
        mainLayout->addLayout(llayout);
        mainLayout->addLayout(rlayout);

        customWidget = new QWidget;
        customWidget->setLayout(mainLayout);
    }

    QWidget *getCustomWidget(){
        return customWidget;
    }

    void refreshNumber(int number){
        QString icon;
        if(number){
            icon = ":images/general/src/GUI/resources/red";
            numberLabel->setText(to_string(number).c_str());
            numberLabel->setVisible(true);
        } else {
            icon = ":images/general/src/GUI/resources/green";
            numberLabel->setVisible(false);
        }
        QIcon ricon;
        QImage rimage(icon);
        ricon.addPixmap(QPixmap::fromImage(rimage), QIcon::Normal, QIcon::On);
        QPixmap rpixmap = ricon.pixmap(QSize(CONTENT_LIST_ITEM_RICON_SIZE, CONTENT_LIST_ITEM_RICON_SIZE), QIcon::Normal, QIcon::On);
        rightIconLabel->setPixmap(rpixmap);
    }

    int getId() { return this->id; }

private:
    QWidget *customWidget;
    QLabel *numberLabel;
    QLabel *rightIconLabel;
    int id;
};

#endif //PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
