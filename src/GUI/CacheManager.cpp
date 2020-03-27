//
// Created by hamzeh on 3/27/20.
//
#include "CacheManager.h"


CacheManager *CacheManager::_instance = nullptr;
QString CacheManager::_cacheDir;

CacheManager * CacheManager::instance() {
    if(_instance==nullptr){
        qCritical() << "PackageManagerTracker is not initiated";
    }
    return _instance;
}

void CacheManager::init(const QString path) {
    if(_instance==nullptr){
        _instance = new CacheManager();
        _cacheDir = path;
    }
}

void CacheManager::clear(){
    clearDir(_cacheDir);
}

QString CacheManager::cacheDir() {
    return _cacheDir;
}

void CacheManager::clearDir(const QString &dir){
//    qDebug() <<  "\n - " << dir;
    QDir _dir(dir);
    QStringList dirs = _dir.entryList(QDir::Dirs);
    for(auto const &d: dirs){
        if(d != "." && d != ".." )
            clearDir(dir+d+"/");
    }

    QStringList files = _dir.entryList(QDir::Files);
    auto currentDate = QDateTime().currentDateTime();
    for(auto const &f: files){
        QFileInfo info(dir+f);
        auto lastRead = info.lastModified();
//        qDebug() << "   " << f << "\t" << lastRead;
        if(lastRead.daysTo(currentDate) > CACHE_EXPIRE_DAY) {
            QFile file(dir+f);
            file.remove();
        }
    }
}
