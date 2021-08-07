#ifndef PX_SOFTWARE_INPROGRESS_ITEM_H
#define PX_SOFTWARE_INPROGRESS_ITEM_H

#include "PXProgressIndicator.h"
#include "PXSideBarItem.h"
#include "PXContentWidget.h"

class InProgressItem : public PXSideBarItem{
public:
    InProgressItem(const QString &title, PXContentWidget *view,
                      QListWidget *parent = nullptr) : PXSideBarItem(title, ItemType::Subitem, view, parent) {
        numberLabel = new QLabel;

        qProgressIndicator = new PXProgressIndicator();
        qProgressIndicator->setFixedSize(20, 20);
        qProgressIndicator->setColor(QGuiApplication::palette().color(QPalette::Active, QPalette::WindowText));
        qProgressIndicator->startAnimation();

        addWidget(numberLabel);
        addWidget(qProgressIndicator);
    }

    void refreshStatus(int number){
        numberLabel->setText(to_string(number).c_str());
    }

private:
    QLabel *numberLabel;
    PXProgressIndicator *qProgressIndicator;
};

#endif //PX_SOFTWARE_INPROGRESS_ITEM_H
