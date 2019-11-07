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

#include "PxQScrollArea.h"
#include "PxQPushButton.h"
#include "DataEntities.h"
#include "DataAccessLayer.h"
#include "PackageWidget.h"

using namespace std;
class CategoryWidget : public QWidget{
    Q_OBJECT
public:
    CategoryWidget(PKG::Category *category);
    PxQScrollArea * getPackageList();
//    QString getName();
//    QString getTitle();
//    QString getDescription();
//    QString getIcon();

private slots:
    void imageDownloaded();

private:
    void loadIcon(QString icon);
    QLabel *iconButton;
    QString name;
    QString title;
    QString description;
    QString icon;
    FileDownloader *m_pImgCtrl;
};


#endif //PX_SOFTWARE_CATEGORYWIDGET_H
