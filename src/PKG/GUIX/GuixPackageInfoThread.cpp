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

#include "GuixPackageInfoThread.h"
#include "MISC/Utils.h"
#include "../RecDB.h"
#include <QDebug>

namespace PKG {
    void GuixPackageInfoThread::run(){
        RecDB db;
        for(auto const &p : m_packageList) {
            bool searchResultFlag = false;
            QString searchCommand = "guix package --show=" + p;
            QString searchResult = QString(PXUTILS::COMMAND::Execute(searchCommand.toStdString().c_str(), 
                                            searchResultFlag).c_str());
            auto recs = db.findFromText(searchResult,"name = '" + p + "'");
            if(recs.size()){
                auto *pkg = Package::MakePackage(recs[0],m_parent);
                if(pkg->title().isEmpty())
                    pkg->setTitle(p);
                packageInfoList.append(pkg);
            }
        }
        emit packageInfoListReady(packageInfoList);
    }
}