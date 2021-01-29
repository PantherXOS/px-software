#ifndef PX_SOFTWARE_UPDATESITEM_H
#define PX_SOFTWARE_UPDATESITEM_H

#include "UserUpdatablePackageListView.h"
#include "PXProgressIndicator.h"
#include "PXSideBarItem.h"
#include "PXContentWidget.h"

class UpdatesItem : public PXSideBarItem{
public:
    UpdatesItem(const QString &title, PXContentWidget *view,
                      QListWidget *parent = nullptr) : PXSideBarItem(title, ItemType::Subitem, view, parent) {
        numberLabel = new QLabel;

        qProgressIndicator = new PXProgressIndicator();
        qProgressIndicator->setFixedSize(UPDATE_ITEM_LOADING_SIZE, UPDATE_ITEM_LOADING_SIZE);
        qProgressIndicator->setColor(QGuiApplication::palette().color(QPalette::Active, QPalette::WindowText));

        rightIconLabel = new QLabel;

        addWidget(qProgressIndicator);
        addWidget(numberLabel);
        addWidget(rightIconLabel);
        startLoadingStatus();
    }

    void startLoadingStatus(){
        qProgressIndicator->startAnimation();
    }

    void refreshStatus(int number){
        qProgressIndicator->stopAnimation();
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
        QPixmap rpixmap = ricon.pixmap(QSize(UPDATE_ITEM_RICON_SIZE, UPDATE_ITEM_RICON_SIZE), QIcon::Normal, QIcon::On);
        rightIconLabel->setPixmap(rpixmap);
    }

private:
    QLabel *numberLabel;
    QLabel *rightIconLabel;
    PXProgressIndicator *qProgressIndicator;
    QSize iconSize = QSize(UPDATE_ITEM_RICON_SIZE,UPDATE_ITEM_RICON_SIZE);
};

#endif //PX_SOFTWARE_UPDATESITEM_H
