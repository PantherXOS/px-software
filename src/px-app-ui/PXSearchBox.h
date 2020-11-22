//
// Created by hamzeh on 12/1/19.
//

#ifndef PX_SOFTWARE_PXSEARCHBOX_H
#define PX_SOFTWARE_PXSEARCHBOX_H
#include <QLineEdit>
#include <QGuiApplication>

#include "PXParamSettings.h"

class PXSearchBox : public QLineEdit{
Q_OBJECT
public:
    PXSearchBox(const QString addressPrefix, QWidget *parent = nullptr) : QLineEdit(parent){
        fixedAddress=addressPrefix;
        connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(searchBoxChanged(const QString &)));
        connect(this, SIGNAL(returnPressed()), this, SLOT(searchBoxHandler()));
        setFixedHeight(SEARCH_BAR_HEIGHT);
//        setFrame(false);
        auto pal = QGuiApplication::palette();
        auto bgcolor = pal.color(QPalette::Active, QPalette::Window);
        auto fgcolor =  pal.color(QPalette::Active, QPalette::WindowText);
        QString sheet = QString::fromLatin1(SEARCH_BAR_STYLE).arg(bgcolor.name(),fgcolor.name());
        setStyleSheet(sheet);
        setFont(QFont(SEARCH_BAR_FONT_NAME, SEARCH_BAR_FONT_SIZE, QFont::Normal));
        clearFocus();
        showMaximized();
    }

    void setAddress(const QString &address){
        QString _str= fixedAddress + "/" + address + "/";
        setPlaceholderText(_str);
        setText(_str);
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

#endif //PX_SOFTWARE_PXSEARCHBOX_H
