//
// Created by Reza Alizadeh Majd on 10/28/19.
//

#ifndef PX_SOFTWARE_TASKRUNNER_H
#define PX_SOFTWARE_TASKRUNNER_H

#include <QObject>
#include <QProcess>
#include <QUuid>

class AsyncTaskRunner : public QObject {
    Q_OBJECT
public:
    explicit AsyncTaskRunner(QObject *parent = nullptr);
    AsyncTaskRunner(QString app, QStringList args, QObject *parent = nullptr);

    virtual bool asyncRun();
    virtual bool asyncRun(const QString &appName, const QStringList &appArgs);
    bool wait();

    QUuid Id() const { return m_id; }
    int timeout() const { return m_timeout; }
    void setTimeout(int timeout) { m_timeout = timeout; }
    QString readAllStandardOutput() const { return m_outStr; }
    QString readAllStandardError() const { return m_errStr; }

signals:
    void done(const QString &outData, const QString &errData);
    void failed(const QString &message);
    void newData(const QString &outData, const QString &errData);

protected:
    QUuid m_id;
    QProcess m_worker;
    QString m_appName;
    QStringList m_appArgs;
    int m_timeout = 30000;
    QString m_outStr;
    QString m_errStr;
};

#endif //PX_SOFTWARE_TASKRUNNER_H
