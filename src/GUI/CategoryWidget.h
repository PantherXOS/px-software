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
#include "PackageListWidgetItem.h"
#include "PackageListWidget.h"

using namespace std;
using namespace PKG;
class CategoryWidget : public QWidget{
    Q_OBJECT
public:
    CategoryWidget(Category *category, QWidget *parent = nullptr);
    Category * getCategory();

private slots:
    void imageDownloaded(const QString & localfile);

private:
    void loadIcon();
    QLabel *iconButton;
    Category *category;
    FileDownloader *m_pImgCtrl;
    QString icon;
};


#endif //PX_SOFTWARE_CATEGORYWIDGET_H
