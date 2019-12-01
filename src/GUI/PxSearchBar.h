//
// Created by hamzeh on 12/1/19.
//

#ifndef PX_SOFTWARE_PXSEARCHBAR_H
#define PX_SOFTWARE_PXSEARCHBAR_H
#include <QLineEdit>

class PxSearchBar : public QLineEdit{
Q_OBJECT
public:
    PxSearchBar(QWidget *parent = nullptr) : QLineEdit(parent){
        connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(searchBoxChanged(const QString &)));
        connect(this, SIGNAL(returnPressed()), this, SLOT(searchBoxHandler()));
        clearFocus();
        showMaximized();
    }

    void setCurrentAddress(const QString &address){
        currentAddress = address;
        setPlaceholderText(address);
        setText(address);
    }


private slots:
    void searchBoxChanged(const QString &usertext) {
        if(text().length() > currentAddress.length()) {
            QString userinput = QString(text().toStdString().substr(currentAddress.length(), text().length() - 1).c_str());
            setText(currentAddress + userinput);
        } else setText(currentAddress);
    };

    void searchBoxHandler(){
        if(text().length() > currentAddress.length()){
            QString userinput = QString(text().toStdString().substr(currentAddress.length(),text().length()-1).c_str());
            emit newUserInputReceived(userinput);
        } else setText(currentAddress);
    }

signals:
    void newUserInputReceived(const QString &userinput);

private:
    QString currentAddress;
};

#endif //PX_SOFTWARE_PXSEARCHBAR_H
