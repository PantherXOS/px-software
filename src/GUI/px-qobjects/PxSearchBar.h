// /*
//  * px-software - Qt-GUI package manager for guix on PantherX OS
//  * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
//  * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
//  *
//  * This program is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  */

// #ifndef PX_SOFTWARE_PXSEARCHBAR_H
// #define PX_SOFTWARE_PXSEARCHBAR_H
// #include <QLineEdit>
// #include "Settings.h"

// class PxSearchBar : public QLineEdit{
// Q_OBJECT
// public:
//     PxSearchBar(QWidget *parent = nullptr) : QLineEdit(parent){
//         connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(searchBoxChanged(const QString &)));
//         connect(this, SIGNAL(returnPressed()), this, SLOT(searchBoxHandler()));
// //        setFrame(false);
//         setFixedHeight(SEARCH_BAR_H);
//         auto pal = QGuiApplication::palette();
//         auto bgcolor = pal.color(QPalette::Active, QPalette::Window);
//         auto fgcolor =  pal.color(QPalette::Active, QPalette::WindowText);
//         QString sheet = QString::fromLatin1(SEARCH_BAR_STYLE).arg(bgcolor.name(),fgcolor.name());
//         setStyleSheet(sheet);
//         setFont(QFont(SEARCH_BAR_FONT_NAME, SEARCH_BAR_FONT_SIZE, QFont::Normal));
//         clearFocus();
//         showMaximized();
//     }

//     void setAddress(const QString &fixed, const QString &var) {
//         fixedAddress = fixed;
//         setPlaceholderText(fixed+var);
//         setText(fixed+var);
//     }

// private slots:
//     void searchBoxChanged(const QString &usertext) {
//         if(text().length() > fixedAddress.length()) {
//             QString userinput = QString(text().toStdString().substr(fixedAddress.length(), text().length() - 1).c_str());
//             setText(fixedAddress + userinput);
//         } else setText(fixedAddress);
//     };

//     void searchBoxHandler(){
//         if(text().length() > fixedAddress.length()){
//             QString userinput = QString(text().toStdString().substr(fixedAddress.length(),text().length()-1).c_str());
//             emit newUserInputReceived(userinput);
//         } else setText(fixedAddress);
//     }

// signals:
//     void newUserInputReceived(const QString &userinput);

// private:
//     QString fixedAddress;
// };

// #endif //PX_SOFTWARE_PXSEARCHBAR_H
