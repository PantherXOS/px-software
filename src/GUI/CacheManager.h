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
