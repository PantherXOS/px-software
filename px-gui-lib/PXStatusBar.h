//
// Created by hamzeh on 1/15/20.
//

#ifndef PX_SOFTWARE_PXSTATUSBAR_H
#define PX_SOFTWARE_PXSTATUSBAR_H

#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <QStatusBar>
#include <QHBoxLayout>
#include <sys/statvfs.h>

#include "PXParamSettings.h"

class PXStatusBar : public QStatusBar{
public:
    PXStatusBar(QWidget *parent= nullptr) : QStatusBar(parent){
        QFont bottomFont("default", STATUSBAR_FONT_SIZE, QFont::Normal);
        QSize size(STATUSBAR_ICON_SIZE, STATUSBAR_ICON_SIZE);

        QString diskSpace;
        if(getFreeDiskSpace(QString("/"), diskSpace)){
            diskSpace += QString(" ") + tr("remaining");
        }
        auto label = new QLabel(this);
        label->setText(diskSpace);
        label->setFont(bottomFont);

        QIcon _icon(QIcon::fromTheme("drive-harddisk"));
        QPixmap pixmap = _icon.pixmap(size, QIcon::Normal, QIcon::On);

        auto iconLabel = new QLabel(this);
        iconLabel->setPixmap(pixmap);
        iconLabel->setFixedSize(size);

        addWidget(iconLabel);
        addWidget(label);
        setFixedWidth(SIDEBAR_WIDTH);
    }

private:
    bool getFreeDiskSpace(QString path, QString &result){
        struct statvfs fiData;
        if((statvfs(path.toStdString().c_str(),&fiData)) < 0 ) {
            result = "Failed to stat " + path;
            return false;
        } else {
    //        printf("Disk %s: \n", "/");
    //        printf("\tblock size: %u\n", fiData.f_bsize);
    //        printf("\ttotal no blocks: %i\n", fiData.f_blocks);
    //        printf("\tfree blocks: %i\n", fiData.f_bfree);
            auto free_kb = (fiData.f_bsize * fiData.f_bfree)/1024;
            float free_gb;
            if(free_kb > 1024){
                auto free_mb = float(free_kb / 1024);
                if(free_mb > 1024){
                    free_gb = float(free_mb / 1024);
                    result = QString::number(free_gb, 'f', 1)+"GB";
                } else result = QString::number(free_mb)+"MB";
            } else
                result = QString::number(free_kb)+"KB";
            return true;
        }
    }
};

#endif //PX_SOFTWARE_PXSTATUSBAR_H
