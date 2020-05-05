//
// Created by Reza Alizadeh Majd on 12/1/19.
//

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
