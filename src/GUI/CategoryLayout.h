//
// Created by hamzeh on 10/22/19.
//

#ifndef PX_SOFTWARE_CATEGORYLAYOUT_H
#define PX_SOFTWARE_CATEGORYLAYOUT_H

#include <iostream>
#include <string>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QImage>
#include <QLabel>
#include <QWidget>
using namespace std;
class CategoryLayout : public QWidget{
public:
    CategoryLayout();
    int getId();
    string getTitle();
    string getDescription();
    string getIcon();

private:
    int id;
    string title;
    string description;
    string icon;
};


#endif //PX_SOFTWARE_CATEGORYLAYOUT_H
