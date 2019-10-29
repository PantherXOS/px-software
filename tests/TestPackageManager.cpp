//
// Created by Reza Alizadeh Majd on 10/26/19.
//

#include <QtTest/QtTest>
#include <PKG/PackageManager.h>

using namespace PKG;

class TestPackageManager : public QObject {
    Q_OBJECT
private slots:
    void getInstalledPackages();

private:
    QString m_dbPath = "./SAMPLE_DB";
};

void TestPackageManager::getInstalledPackages() {
    PackageManager packageMgr(m_dbPath, this);
    QSignalSpy spy(&packageMgr, &PackageManager::installedPackagesReady);
    QSignalSpy spyError(&packageMgr, &PackageManager::failed);
    packageMgr.requestInstalledPackages();
    while (!spy.wait() && !spyError.wait()) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
}


QTEST_GUILESS_MAIN(TestPackageManager)
#include "TestPackageManager.moc"
