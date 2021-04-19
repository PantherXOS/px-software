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


#ifndef PX_SOFTWARE_GUIXPACKAGEINFOTHREAD_H
#define PX_SOFTWARE_GUIXPACKAGEINFOTHREAD_H

#include <QThread>
#include <QString>
#include "DataEntities.h"

namespace PKG {
    class GuixPackageInfoThread : public QThread {
        Q_OBJECT
    public:
        GuixPackageInfoThread(const QStringList &packageList, QObject *parent = nullptr) : 
            QThread(parent), 
            m_packageList(packageList){
                m_parent = parent;
            }
        void run() override;

    signals:
        void packageInfoListReady(const QVector<Package *> &);
    
    private: 
        QObject            *m_parent;
        QStringList         m_packageList;
        QVector <Package *> packageInfoList;
    };
}

#endif //PX_SOFTWARE_GUIXPACKAGEINFOTHREAD_H
