//
// Created by Reza Alizadeh Majd on 10/26/19.
//

#include <QtTest/QtTest>
#include <PKG/PackageManager.h>

using namespace PKG;

class TestPackageManager : public QObject {
    Q_OBJECT
public:
    explicit TestPackageManager(QObject *parent = nullptr);

private slots:
    void getInstalledPackages();
    void getUserUpgradablePackages();

private:
    QString m_dbPath = "./SAMPLE_DB";
    PackageManager *m_pkgMgr = nullptr;
};

TestPackageManager::TestPackageManager(QObject *parent) : QObject(parent) {
    m_pkgMgr = new PackageManager(m_dbPath, this);
}

void TestPackageManager::getInstalledPackages() {
    QSignalSpy spy(m_pkgMgr, &PackageManager::installedPackagesReady);
    QSignalSpy spyError(m_pkgMgr, &PackageManager::failed);
    m_pkgMgr->requestInstalledPackages();
    while (!spy.wait() && !spyError.wait()) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
}

void TestPackageManager::getUserUpgradablePackages() {
    QSignalSpy spy(m_pkgMgr, &PackageManager::userUpgradablePackagesReady);
    QSignalSpy spyError(m_pkgMgr, &PackageManager::failed);
    m_pkgMgr->requestUserUpgradablePackages();
    while (!spy.wait() && !spyError.wait()) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
}

QTEST_GUILESS_MAIN(TestPackageManager)
#include "TestPackageManager.moc"
