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

#include "GuixWrapper.h"
#include <QDebug>

namespace PKG {

    GuixWrapper::GuixWrapper(QObject *parent) : QObject(parent), m_currentTask(nullptr) {
        connect(&m_timer, &QTimer::timeout, this, &GuixWrapper::execNextTask);
        m_timer.setInterval(500);
    }

    void GuixWrapper::start() {
        m_timer.start();
    }

    void GuixWrapper::stop() {
        m_timer.stop();
    }

    bool GuixWrapper::appendTask(AsyncTaskRunner *task, bool force) {
        QMutexLocker locker(&m_lock);
        if (task != nullptr) {
            if (force) {
                m_taskQueue.prepend(task);
            } else {
                m_taskQueue.append(task);
            }
            return true;
        }
        return false;
    }

    bool GuixWrapper::cancelTask(const QUuid &taskId) {
        QMutexLocker locker(&m_lock);
        // Check running task.
        if (m_currentTask != nullptr && m_currentTask->Id() == taskId) {
            m_currentTask->cancel();
            return true;
        }
        // search between pending tasks
        for (int i = 0; i < m_taskQueue.size(); i++) {
            if (m_taskQueue[i]->Id() == taskId) {
                m_taskQueue[i]->cancel();
                m_taskQueue.remove(i);
                return true;
            }
        }
        // do we need to return true in both cases?
        return true;
    }

    void GuixWrapper::execNextTask() {
        m_currentTask = nullptr;
        if (!m_taskQueue.isEmpty()) {
            QMutexLocker locker(&m_lock);
            m_currentTask = m_taskQueue.takeFirst();
        }
        if (m_currentTask != nullptr) {
            connect(m_currentTask, &AsyncTaskRunner::done, [=]() { m_timer.start(); });
            connect(m_currentTask, &AsyncTaskRunner::failed, [=]() { m_timer.start(); });
            connect(m_currentTask, &AsyncTaskRunner::canceled, [=]() { m_timer.start(); });
            connect(m_currentTask, &AsyncTaskRunner::done, &QObject::deleteLater);
            connect(m_currentTask, &AsyncTaskRunner::failed, &QObject::deleteLater);
            m_timer.stop();
//            qDebug() << m_currentTask->Id() << m_currentTask->metaObject()->className();
            m_currentTask->asyncRun();
        }
    }
}
