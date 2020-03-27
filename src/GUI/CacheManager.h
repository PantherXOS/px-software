//
// Created by hamzeh on 3/27/20.
//

#ifndef PX_SOFTWARE_CACHEMANAGER_H
#define PX_SOFTWARE_CACHEMANAGER_H

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QFile>
#include <QDateTime>

#include "Settings.h"

class CacheManager {
public:
    static CacheManager * instance();
    static void init(const QString path);
    void clear();
    QString cacheDir();

private:
    CacheManager(){}
    void clearDir(const QString &dir);

    static CacheManager *_instance;
    static QString _cacheDir;
};
#endif //PX_SOFTWARE_CACHEMANAGER_H
