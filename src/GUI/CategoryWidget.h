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
    PxQScrollArea * getPackageList();
//    QString getName();
//    QString getTitle();
//    QString getDescription();
//    QString getIcon();

private slots:
    void imageDownloaded();
    void categoryPackagesReadyHandler(const QUuid &taskId, const QVector<Package *> & packages);
    void taskFailedHandler(const QUuid &, const QString &);

private:
    void loadIcon();
    QLabel *iconButton;
    QString name;
    QString title;
    QString description;
    QString icon;
    FileDownloader *m_pImgCtrl;
    PackageListWidget *packageListWidget;
};


#endif //PX_SOFTWARE_CATEGORYWIDGET_H
