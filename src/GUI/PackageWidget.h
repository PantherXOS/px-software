//
// Created by hamzeh on 11/3/19.
//

#ifndef PX_SOFTWARE_PACKAGEWIDGET_H
#define PX_SOFTWARE_PACKAGEWIDGET_H
#include <iostream>
#include <string>
#include <QWidget>
#include <QString>
#include <vector>
#include <QLabel>
#include <QImage>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "DataEntities.h"

using namespace std;
class PackageWidget :public QWidget {
    Q_OBJECT
public:
    PackageWidget(PKG::Package *package);

private slots:
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();

private:
    QString name;
    QString title;
    QString version;
    QString description;
    QString homepage;
    QString license;
    QString icon;
    QStringList screenshotsList;
    QStringList categoriesList;
};


#endif //PX_SOFTWARE_PACKAGEWIDGET_H
