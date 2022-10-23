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

#include "LxqtSessionInterface.h"

#define LXQT_SESSION_SERVICE             "org.lxqt.session"
#define LXQT_SESSION_PATH                "/LXQtSession"
#define LXQT_SESSION_MODULE_PANEL        "lxqt-panel.desktop"
#define LXQT_SESSION_METHOD_MODULE_START "startModule"
#define LXQT_SESSION_METHOD_MODULE_STOP  "stopModule"

LxqtSessionInterface::LxqtSessionInterface(QObject *parent) : QObject(parent) {
    m_interface = new QDBusInterface(LXQT_SESSION_SERVICE,
                                     LXQT_SESSION_PATH,
                                     QString(),
                                     QDBusConnection::sessionBus(),
                                     this);
    connect(m_interface,
            SIGNAL(moduleStateChanged(QString, bool)),
            SLOT(handleModuleState(const QString &, bool)));
}

void LxqtSessionInterface::changePanelState(bool running) {
    QList<QVariant> arg;
    arg.append(LXQT_SESSION_MODULE_PANEL);
    m_interface->callWithArgumentList(
            QDBus::NoBlock,
            (running ? LXQT_SESSION_METHOD_MODULE_START : LXQT_SESSION_METHOD_MODULE_STOP),
            arg);
}

void LxqtSessionInterface::startPanel() {
    changePanelState(true);
}

void LxqtSessionInterface::stopPanel() {
    changePanelState(false);
}

void LxqtSessionInterface::restartPanel() {
    auto *stopConn = new QMetaObject::Connection();
    auto *startConn = new QMetaObject::Connection();
    *stopConn = connect(this, &LxqtSessionInterface::panelStopped, &LxqtSessionInterface::startPanel);
    *startConn = connect(this, &LxqtSessionInterface::panelStarted, [=]() {
        disconnect(*stopConn);
        disconnect(*startConn);
        delete stopConn;
        delete startConn;
        emit panelRestarted();
    });
    stopPanel();
    QThread::sleep(1);
}

void LxqtSessionInterface::handleModuleState(const QString &moduleName, bool state) {
    if (moduleName == LXQT_SESSION_MODULE_PANEL) {
        emit (state ? panelStarted() : panelStopped());
    }
}
