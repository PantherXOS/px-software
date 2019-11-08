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
#include <QLineEdit>

#include "DataEntities.h"
#include "FileDownloader.h"

using namespace std;
class PackageWidget :public QWidget {
    Q_OBJECT
public:
    PackageWidget(PKG::Package *package,bool installEnable, bool updateEnable, bool removeEnable);

private slots:
    void imageDownloaded();
    void installButtonHandler();
    void removeButtonHandler();
    void updateButtonHandler();

private:
    QHBoxLayout *loadIcon();
    QVBoxLayout *loadTexts();
    QHBoxLayout *loadButtons(bool installEnable, bool updateEnable, bool removeEnable);
    QPushButton *updateButton, *removeButton, *installButton;
    PKG::Package *package;
    QLabel *iconButton;
    QUrl iconRemoteUrl;
    FileDownloader *m_pImgCtrl;
};


#endif //PX_SOFTWARE_PACKAGEWIDGET_H
