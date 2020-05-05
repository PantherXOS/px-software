//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_SYSTEMUPDATABLEWIDGETITEM_H
#define PX_SOFTWARE_SYSTEMUPDATABLEWIDGETITEM_H
#include "PxQScrollArea.h"
#include "SystemUpdatablePackageListView.h"
#include "PxQListWidgetItem.h"

class SystemUpdatableWidgetItem : public PxQListWidgetItem{
public:
    SystemUpdatableWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                              QListWidget *parent = nullptr) : PxQListWidgetItem(id, title, font,
                                                                                 iconItemFile, parent) {
        buildRightLayout();
        SystemUpdatablePackageListView::init(title);
        view = SystemUpdatablePackageListView::Instance();
    }

    PxQScrollArea *getView() override{
        view->refresh();
        return view;
    }

    void buildRightLayout(){
        startLoadingStatus();
        numberLabel = new QLabel;
        QFont font = numberLabel->font();
        font.setBold(true);
        numberLabel->setFont(font);
        rightLayout()->addWidget(numberLabel);
        rightLayout()->addWidget(rightIconLabel);
    }

    void startLoadingStatus(){
        rightIconLabel = new PxCircleLoadingAnimation(QSize(CONTENT_LIST_ITEM_RICON_SIZE, CONTENT_LIST_ITEM_RICON_SIZE));
    }

    void refreshStatus(int number){
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
    SystemUpdatablePackageListView *view;
    QLabel *numberLabel;
    QLabel *rightIconLabel;
};
#endif //PX_SOFTWARE_SYSTEMUPDATABLEWIDGETITEM_H
