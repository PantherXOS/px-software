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
#include "PxQScrollArea.h"
#include "PxCircleLoadingAnimation.h"

using namespace std;
class PxQListWidgetItem : public QListWidgetItem
{
public:
    PxQListWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                      QListWidget *parent = nullptr) : QListWidgetItem(parent){
        auto titleLabel = new QLabel(title);
        titleLabel->setFont(font);

        QIcon qicon(QIcon::fromTheme(iconItemFile));
        QPixmap pixmap = qicon.pixmap(QSize(CONTENT_LIST_ICON_SIZE, CONTENT_LIST_ICON_SIZE), QIcon::Normal, QIcon::On);
        auto iconItem = new QLabel;
        iconItem->setPixmap(pixmap);

        auto llayout = new QHBoxLayout;
        llayout->addWidget(iconItem);
        llayout->addWidget(titleLabel);
        llayout->setAlignment(Qt::AlignLeft);

        rlayout = new QHBoxLayout;
        rlayout->setAlignment(Qt::AlignRight);

        auto mainLayout = new QHBoxLayout;
        mainLayout->addLayout(llayout);
        mainLayout->addLayout(rlayout);
        mainLayout->setContentsMargins(10,10,mainLayout->contentsMargins().right(),5);

        customWidget = new QWidget;
        customWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        customWidget->setLayout(mainLayout);
    }

    QWidget *getCustomWidget(){
        return customWidget;
    }

    virtual PxQScrollArea *getView(){
        return view;
    }

    QHBoxLayout *rightLayout (){
        return rlayout;
    }

private:
    QHBoxLayout *rlayout;
    QWidget *customWidget;
    PxQScrollArea *view;
};

#endif //PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
