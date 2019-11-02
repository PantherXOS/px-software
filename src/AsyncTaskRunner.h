//
// Created by Reza Alizadeh Majd on 10/28/19.
//

#ifndef PX_SOFTWARE_TASKRUNNER_H
#define PX_SOFTWARE_TASKRUNNER_H

#include <QObject>
#include <QProcess>

class AsyncTaskRunner : public QObject {
    Q_OBJECT
public:
    explicit AsyncTaskRunner(QObject *parent = nullptr);
    bool asyncRun(const QString &appName, const QStringList &args = QStringList());
    bool wait();

    int timeout() const { return m_timeout; }
    void setTimeout(int timeout) { m_timeout = timeout; }

signals:
    void done(const QString &result);
    void failed(const QString &message);

private:
    QProcess m_worker;
    int m_timeout = 30000;
};

#endif //PX_SOFTWARE_TASKRUNNER_H
