//
// Created by hamzeh on 12/1/19.
//

#ifndef PX_SOFTWARE_PXSEARCHBAR_H
#define PX_SOFTWARE_PXSEARCHBAR_H
#include <QLineEdit>
#include "Settings.h"

class PxSearchBar : public QLineEdit{
Q_OBJECT
public:
    PxSearchBar(QWidget *parent = nullptr) : QLineEdit(parent){
        connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(searchBoxChanged(const QString &)));
        connect(this, SIGNAL(returnPressed()), this, SLOT(searchBoxHandler()));
        setFrame(false);
        setFixedHeight(SEARCH_BAR_H);
        clearFocus();
        showMaximized();
    }

    void setAddress(const QString &fixed, const QString &var) {
        fixedAddress = fixed;
        setPlaceholderText(fixed+var);
        setText(fixed+var);
    }

private slots:
    void searchBoxChanged(const QString &usertext) {
        if(text().length() > fixedAddress.length()) {
            QString userinput = QString(text().toStdString().substr(fixedAddress.length(), text().length() - 1).c_str());
            setText(fixedAddress + userinput);
        } else setText(fixedAddress);
    };

    void searchBoxHandler(){
        if(text().length() > fixedAddress.length()){
            QString userinput = QString(text().toStdString().substr(fixedAddress.length(),text().length()-1).c_str());
            emit newUserInputReceived(userinput);
        } else setText(fixedAddress);
    }

signals:
    void newUserInputReceived(const QString &userinput);

private:
    QString fixedAddress;
};

#endif //PX_SOFTWARE_PXSEARCHBAR_H
