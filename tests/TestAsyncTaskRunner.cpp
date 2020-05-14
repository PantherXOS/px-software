/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
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