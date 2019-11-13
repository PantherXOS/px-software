//
// Created by Reza Alizadeh Majd on 10/28/19.
//

#include "AsyncTaskRunner.h"
#include <QDateTime>
#include <QDebug>
#include <utility>

AsyncTaskRunner::AsyncTaskRunner(QObject *parent) : AsyncTaskRunner(QString(), QStringList(), parent) {}

AsyncTaskRunner::AsyncTaskRunner(QString app, QStringList args, QObject *parent) :
        QObject(parent),
        m_appName(std::move(app)),
        m_appArgs(std::move(args)),
        m_id(QUuid::createUuid()) {
    connect(&m_worker, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [&](int exitCode, QProcess::ExitStatus exitStatus) {
                m_outStr += m_worker.readAllStandardOutput();
                m_errStr += m_worker.readAllStandardError();
                if (exitStatus == QProcess::NormalExit && exitCode == 0) {
                    emit done(m_outStr, m_errStr);
                } else {
                    emit failed(QString("process execution failed: (%1) %2").arg(exitCode).arg(m_worker.errorString()));
                }
            });

    connect(&m_worker, &QProcess::errorOccurred, [&](QProcess::ProcessError error) {
        emit failed(QString("error occurred on execution process: %1").arg(m_worker.errorString()));
    });

    connect(&m_worker, &QProcess::readyReadStandardOutput, [&]() {
        m_worker.setReadChannel(QProcess::StandardOutput);
        while (m_worker.canReadLine()) {
            QString data = m_worker.readLine();
            m_outStr += data;
            emit newData(data, QString());
        }
    });
    connect(&m_worker, &QProcess::readyReadStandardError, [&]() {
        m_worker.setReadChannel(QProcess::StandardError);
        while (m_worker.canReadLine()) {
            QString data = m_worker.readLine();
            m_errStr += data;
            emit newData(QString(), data);
        }
    });
}

bool AsyncTaskRunner::asyncRun() {
    return this->asyncRun(m_appName, m_appArgs);
}

bool AsyncTaskRunner::asyncRun(const QString &appName, const QStringList &appArgs) {
    QString app = appName.isEmpty() ? m_appName : appName;
    QStringList args = appName.isEmpty() ? m_appArgs : appArgs;
    m_worker.start(app, args);
    return m_worker.waitForStarted(m_timeout);
}

bool AsyncTaskRunner::wait() {
    return m_worker.waitForFinished();
}
