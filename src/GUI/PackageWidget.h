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
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "DataEntities.h"
#include "FileDownloader.h"

using namespace std;
class PackageWidget :public QWidget {
    Q_OBJECT
public:
    PackageWidget(PKG::Package *package);

private slots:
    void imageDownloaded();
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();

private:
    void loadIcon();
    QLabel *iconButton;
    QString name;
    QString title;
    QString version;
    QString description;
    QString homepage;
    QString license;
    QUrl iconRemoteUrl;
    QStringList screenshotsList;
    QStringList categoriesList;
    FileDownloader *m_pImgCtrl;
};


#endif //PX_SOFTWARE_PACKAGEWIDGET_H
