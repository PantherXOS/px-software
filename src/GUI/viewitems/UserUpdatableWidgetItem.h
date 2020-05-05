//
// Created by hamzeh on 12/17/19.
//

#ifndef PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
#define PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
#include <QFont>

#include "PxQScrollArea.h"
#include "UserUpdatablePackageListView.h"
#include "PxQListWidgetItem.h"
#include "QProgressIndicator.h"

class UserUpdatableWidgetItem : public PxQListWidgetItem{
public:
    UserUpdatableWidgetItem(const QString &id, const QString &title, const QFont &font, const QString &iconItemFile,
                            QListWidget *parent = nullptr) : PxQListWidgetItem(id, title, font,
                                                                               iconItemFile, parent) {
        buildRightLayout();
        UserUpdatablePackageListView::init(title);
        view = UserUpdatablePackageListView::Instance();
    }

    PxQScrollArea *getView() override{
//        startLoadingStatus();
        view->refresh();
        return view;
    }

    void buildRightLayout(){
        numberLabel = new QLabel;
        QFont font = numberLabel->font();
        font.setBold(true);
        numberLabel->setFont(font);

        qProgressIndicator = new QProgressIndicator();
        qProgressIndicator->setFixedSize(CONTENT_LIST_LOADING_SIZE, CONTENT_LIST_LOADING_SIZE);
        qProgressIndicator->setColor(QGuiApplication::palette().color(QPalette::Active, QPalette::WindowText));

        rightIconLabel = new QLabel;
        rightLayout()->addWidget(qProgressIndicator);
        rightLayout()->addWidget(numberLabel);
        rightLayout()->addWidget(rightIconLabel);
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
    UserUpdatablePackageListView *view;
    QLabel *numberLabel;
    QLabel *rightIconLabel;
    QProgressIndicator *qProgressIndicator;
};

#endif //PX_SOFTWARE_USERUPDATABLEWIDGETITEM_H
