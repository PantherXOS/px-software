//
// Created by hamzeh on 10/22/19.
//

#ifndef PX_SOFTWARE_CATEGORYWIDGET_H
#define PX_SOFTWARE_CATEGORYWIDGET_H

#include <iostream>
#include <string>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QImage>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QUrl>
#include <QRegExp>

#include "DataEntities.h"

using namespace std;
class CategoryWidget : public QWidget{
public:
    CategoryWidget(PKG::Category *category);
    string getName();
    string getTitle();
    string getDescription();
    string getIcon();

private:
    string name;
    string title;
    string description;
    string icon;
};


#endif //PX_SOFTWARE_CATEGORYWIDGET_H
