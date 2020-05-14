/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

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
