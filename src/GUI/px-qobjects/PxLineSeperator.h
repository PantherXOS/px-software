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

#ifndef PX_SOFTWARE_PXLINESEPERATOR_H
#define PX_SOFTWARE_PXLINESEPERATOR_H
#include <QFrame>

using namespace std;
class PxLineSeperator : public QFrame{
    Q_OBJECT
public:
    PxLineSeperator(QWidget *parent = nullptr) : QFrame(parent){
        setGeometry(QRect(320, 150, 118, 1));
        setFrameShape(QFrame::HLine);
        setFrameShadow(QFrame::Sunken);
        setStyleSheet("QFrame{background-color: transparent}");
    }
};
#endif //PX_SOFTWARE_PXLINESEPERATOR_H
