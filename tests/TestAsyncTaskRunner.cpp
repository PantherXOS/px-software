//
// Created by Reza Alizadeh Majd on 10/29/19.
//

#include <QtTest>
#include <AsyncTaskRunner.h>

class TestAsyncTaskRunner : public QObject {
    Q_OBJECT
private slots:
    void asyncSuccess();
    void asyncFailure();
};

void TestAsyncTaskRunner::asyncSuccess() {
    QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner;
    QSignalSpy spy(worker, &AsyncTaskRunner::done);
    QVERIFY(worker->asyncRun("ls", QStringList() << "-la"));
    worker->wait();
    QCOMPARE(spy.count(), 1);
}

void TestAsyncTaskRunner::asyncFailure() {
    QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner;
    QSignalSpy spy(worker, &AsyncTaskRunner::failed);
    QVERIFY(!worker->asyncRun("non_existent_application", QStringList() << "-la"));
    worker->wait();
    QCOMPARE(spy.count(), 1);
}

QTEST_GUILESS_MAIN(TestAsyncTaskRunner)
#include <TestAsyncTaskRunner.moc>