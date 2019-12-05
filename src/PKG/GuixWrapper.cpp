//
// Created by Reza Alizadeh Majd on 12/1/19.
//

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

    bool GuixWrapper::appendTask(GuixTask *task, bool force) {
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
