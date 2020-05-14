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

#ifndef PX_SOFTWARE_GUIXWRAPPER_H
#define PX_SOFTWARE_GUIXWRAPPER_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include "GUIX/GuixTask.h"

namespace PKG {
    class GuixWrapper : public QObject {
        Q_OBJECT
    public:
        explicit GuixWrapper(QObject *parent = nullptr);
        void start();
        void stop();

    public slots:
        bool appendTask(AsyncTaskRunner *task, bool force = false);
        bool cancelTask(const QUuid &taskId);

    protected slots:
        void execNextTask();

    private:
        QTimer m_timer;
        QMutex m_lock;
        AsyncTaskRunner *m_currentTask;
        QVector<AsyncTaskRunner *> m_taskQueue;
    };
}

#endif //PX_SOFTWARE_GUIXWRAPPER_H
