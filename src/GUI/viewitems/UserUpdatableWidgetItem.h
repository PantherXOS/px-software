//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
#define PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
#include "src/GUI/px-qobjects/PxQScrollArea.h"
#include "UserUpdatablePackageListView.h"
#include "src/GUI/px-qobjects/PxQListWidgetItem.h"
#include "src/GUI/px-qobjects/PxCircleLoadingAnimation.h"

class UserUpdatableWidgetItem : public PxQListWidgetItem{
public:
    UserUpdatableWidgetItem(const QString &title, const QFont &font, const QString &iconItemFile,
                            QListWidget *parent = nullptr) : PxQListWidgetItem(title, font, iconItemFile, parent) {
        buildRightLayout();
        UserUpdatablePackageListView::init(title);
        view = UserUpdatablePackageListView::Instance();
        view->refresh();
    }

    PxQScrollArea *getView() override{
        view->refresh();
        return view;
    }

    void buildRightLayout(){
        rightIconLabel = new PxCircleLoadingAnimation(QSize(CONTENT_LIST_ITEM_RICON_SIZE, CONTENT_LIST_ITEM_RICON_SIZE));
        numberLabel = new QLabel;
        rightLayout()->addWidget(numberLabel);
        rightLayout()->addWidget(rightIconLabel);
    }

    void refresh(int number){
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
    UserUpdatablePackageListView *view;
    QLabel *numberLabel;
    QLabel *rightIconLabel;
};

#endif //PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
