#ifndef PX_SOFTWARE_UPDATESITEM_H
#define PX_SOFTWARE_UPDATESITEM_H

#include "UserUpdatablePackageListView.h"
#include "QProgressIndicator.h"
#include "PXSideBarItem.h"
#include "PXScrollArea.h"

class UpdatesItem : public PXSideBarItem{
public:
    UpdatesItem(const QString &title, PXScrollArea *view,
                      QListWidget *parent = nullptr) : PXSideBarItem(title, ItemType::Subitem, view, parent) {
        numberLabel = new QLabel;
        QFont font = numberLabel->font();
        font.setBold(true);
        numberLabel->setFont(font);

        qProgressIndicator = new QProgressIndicator();
        qProgressIndicator->setFixedSize(CONTENT_LIST_LOADING_SIZE, CONTENT_LIST_LOADING_SIZE);
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
        QPixmap rpixmap = ricon.pixmap(QSize(CONTENT_LIST_ITEM_RICON_SIZE, CONTENT_LIST_ITEM_RICON_SIZE), QIcon::Normal, QIcon::On);
        rightIconLabel->setPixmap(rpixmap);
    }

private:
    QLabel *numberLabel;
    QLabel *rightIconLabel;
    QProgressIndicator *qProgressIndicator;
    QSize iconSize = QSize(CONTENT_LIST_ITEM_RICON_SIZE,CONTENT_LIST_ITEM_RICON_SIZE);
};

#endif //PX_SOFTWARE_UPDATESITEM_H
