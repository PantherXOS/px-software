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

#include "GuixProfileUpdateThread.h"
#include "MISC/Utils.h"
#include <QDebug>

namespace PKG {
    void GuixProfileUpdateThread::run(){
        bool resultFlag = false;
        string command = "guix pull --disable-authentication 2>&1";
        QString result = QString(PXUTILS::COMMAND::Execute(command.c_str(), resultFlag).c_str());
        if(resultFlag)
            qDebug() << result;
        else
            emit error(QString(result));
    }
}