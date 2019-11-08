//
// Created by Reza Alizadeh Majd on 10/26/19.
//

#include <QtTest/QtTest>
#include <PKG/PackageManager.h>
#include <QDebug>

using namespace PKG;

class TestPackageManager : public QObject {
    Q_OBJECT
public:
    explicit TestPackageManager(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void getInstalledPackages();
    void getUserUpgradablePackages();
    void getSystemUpgradablePackages();
    void installPackage();

private:
    QString m_dbPath = "./SAMPLE_DB";
    PackageManager *m_pkgMgr = nullptr;
};

void TestPackageManager::initTestCase() {
    // cleanup environment for test execution

    // 1. uninstall `hello` package
    QProcess process(this);
    process.start("guix", QStringList() << "package" << "-r" << "hello");
    process.waitForStarted();
    process.waitForFinished(60000);
}

TestPackageManager::TestPackageManager(QObject *parent) : QObject(parent) {
    m_pkgMgr = new PackageManager(m_dbPath, this);
}

void TestPackageManager::getInstalledPackages() {
    QSignalSpy spy(m_pkgMgr, &PackageManager::installedPackagesReady);
    QSignalSpy spyError(m_pkgMgr, &PackageManager::failed);
    m_pkgMgr->requestInstalledPackages();
    while (!(spy.count() > 0
             || spy.wait()
             || spyError.count() > 0
             || spyError.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
}

void TestPackageManager::getUserUpgradablePackages() {
    QSignalSpy spy(m_pkgMgr, &PackageManager::userUpgradablePackagesReady);
    QSignalSpy spyError(m_pkgMgr, &PackageManager::failed);
    m_pkgMgr->requestUserUpgradablePackages();
    while (!(spy.count() > 0
             || spy.wait()
             || spyError.count() > 0
             || spyError.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
}

void TestPackageManager::getSystemUpgradablePackages() {
    QSignalSpy spy(m_pkgMgr, &PackageManager::systemUpgradablePackagesReady);
    QSignalSpy spyError(m_pkgMgr, &PackageManager::failed);
    m_pkgMgr->requestSystemUpgradablePackages();
    while (!(spy.count() > 0
             || spy.wait()
             || spyError.count() > 0
             || spyError.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
}

void TestPackageManager::installPackage() {
    QString packageName = "hello";
    QSignalSpy spy(m_pkgMgr, &PackageManager::packageInstalled);
    QSignalSpy spyError(m_pkgMgr, &PackageManager::failed);
    QSignalSpy spyInstallLog(m_pkgMgr, &PackageManager::newTaskData);
    QObject::connect(m_pkgMgr, &PackageManager::newTaskData,
                     [&](const QUuid &taskId, const QString &data) {
                         qDebug() << QString("new data for %1: %2")
                                 .arg(taskId.toString())
                                 .arg(data);
                     });
    m_pkgMgr->requestPackageInstallation(packageName);
    while (!(spy.count() > 0
             || spy.wait()
             || spyError.count() > 0
             || spyError.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
    QVERIFY(spyInstallLog.count() > 0);
}

QTEST_GUILESS_MAIN(TestPackageManager)
#include "TestPackageManager.moc"
