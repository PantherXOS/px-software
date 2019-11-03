//
// Created by Reza Alizadeh Majd on 10/28/19.
//

#include "AsyncTaskRunner.h"

AsyncTaskRunner::AsyncTaskRunner(QObject *parent) : QObject(parent) {
    connect(&m_worker,
            static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [&](int exitCode, QProcess::ExitStatus exitStatus) {
                if (exitStatus == QProcess::NormalExit) {
                    emit done(m_worker.readAllStandardOutput(), m_worker.readAllStandardError());
                } else {
                    emit failed(QString("process failed with %1 code.").arg(exitCode), m_worker.readAllStandardError());
                }
                this->deleteLater();
            });

    connect(&m_worker,
            static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
            [&](QProcess::ProcessError error) {
                emit failed(m_worker.readAllStandardOutput(), m_worker.readAllStandardError());
                this->deleteLater();
            });
}

bool AsyncTaskRunner::asyncRun(const QString &appName, const QStringList &args) {
    m_worker.start(appName, args);
    return m_worker.waitForStarted(m_timeout);

}

bool AsyncTaskRunner::wait() {
    return m_worker.waitForFinished();
}
