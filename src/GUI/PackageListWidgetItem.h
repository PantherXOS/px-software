//
// Created by hamzeh on 11/3/19.
//

#ifndef PX_SOFTWARE_PACKAGELISTWIDGETITEM_H
#define PX_SOFTWARE_PACKAGELISTWIDGETITEM_H
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
#include "PackageManager.h"
#include "PackageManagerTracker.h"
#include "TerminalWidget.h"
#include "PackageComponent.h"

using namespace std;
using namespace PKG;
class PackageListWidgetItem :public QWidget {
    Q_OBJECT
public:
    PackageListWidgetItem(Package *package, bool removeEnable, QWidget *parent = nullptr);
    Package * & getPackage();
    TerminalWidget * getTerminal();

private:
    QVBoxLayout *loadTexts();

    Package *package;
    PackageComponent *packageComponent;
    PackageManagerTracker *m_pkgMgrTrk = nullptr;
};


#endif //PX_SOFTWARE_PACKAGELISTWIDGETITEM_H
