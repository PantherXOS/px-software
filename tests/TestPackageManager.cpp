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
    ~TestPackageManager() override;

private slots:
    void initTestCase();
    void init();
    void getInstalledPackages();
    void getUserUpgradablePackages();
    void getSystemUpgradablePackages();
    void getCategoryPackages();
    void installPackage();
    void updatePackage();
    void removePackage();

private:
    void enableDebug() { m_printDebug = true; }
    void disableDebug() { m_printDebug = false; }

private:
    QString m_dbPath = "./SAMPLE_DB";
    PackageManager *m_pkgMgr = nullptr;
    bool m_printDebug = false;
};

void TestPackageManager::initTestCase() {
    // cleanup environment for test execution
    QObject::connect(m_pkgMgr, &PackageManager::newTaskData, [=](const QUuid &taskId, const QString &data) {
        if (m_printDebug) {
            qDebug() << QString("new data for %1: %2")
                    .arg(taskId.toString())
                    .arg(data);
        }
    });

    // 1. uninstall `hello` package
    QProcess process(this);
    process.start("guix", QStringList() << "package" << "-r" << "hello");
    process.waitForStarted();
    process.waitForFinished(60000);
}

void TestPackageManager::init() {
    this->disableDebug();
}

TestPackageManager::TestPackageManager(QObject *parent) : QObject(parent) {
    PackageManager::Init(m_dbPath, this);
    m_pkgMgr = PackageManager::Instance();
}

TestPackageManager::~TestPackageManager() {
    PackageManager::Destruct();
}

void TestPackageManager::getInstalledPackages() {
    QSignalSpy spy(m_pkgMgr, &PackageManager::installedPackagesReady);
    connect(m_pkgMgr, &PackageManager::installedPackagesReady, [=](const QVector<Package *> &pkgList) {
        QVERIFY(pkgList.count() > 0);
    });
    QSignalSpy spyError(m_pkgMgr, &PackageManager::taskFailed);
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
    QSignalSpy spyError(m_pkgMgr, &PackageManager::taskFailed);
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
    QSignalSpy spyError(m_pkgMgr, &PackageManager::taskFailed);
    m_pkgMgr->requestSystemUpgradablePackages();
    while (!(spy.count() > 0
             || spy.wait()
             || spyError.count() > 0
             || spyError.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
}

void TestPackageManager::getCategoryPackages() {
    QString categoryName = "development";
    QSignalSpy spy(m_pkgMgr, &PackageManager::categoryPackagesReady);
    QSignalSpy spyErr(m_pkgMgr, &PackageManager::taskFailed);
    m_pkgMgr->requestCategoryPackages(categoryName);
    while (!(spy.count() > 0
             || spy.wait()
             || spyErr.count() > 0
             || spyErr.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyErr.count(), 0);
}

void TestPackageManager::installPackage() {
    QString packageName = "hello";
    QSignalSpy spy(m_pkgMgr, &PackageManager::packageInstalled);
    QSignalSpy spyError(m_pkgMgr, &PackageManager::taskFailed);
    QSignalSpy spyInstallLog(m_pkgMgr, &PackageManager::newTaskData);
    m_pkgMgr->requestPackageInstallation(packageName);
    while (!(spy.count() > 0
             || spy.wait()
             || spyError.count() > 0
             || spyError.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyError.count(), 0);
    QVERIFY(spyInstallLog.count() > 0);
}

void TestPackageManager::updatePackage() {
    QString packageName = "hello";
    QSignalSpy spy(m_pkgMgr, &PackageManager::packageUpdated);
    QSignalSpy spyErr(m_pkgMgr, &PackageManager::taskFailed);
    QSignalSpy spyLog(m_pkgMgr, &PackageManager::newTaskData);
    m_pkgMgr->requestPackageUpdate(QStringList() << packageName);
    while (!(spy.count() > 0
             || spy.wait()
             || spyErr.count() > 0
             || spyErr.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyErr.count(), 0);
    QVERIFY(spyLog.count() >= 0);
}

void TestPackageManager::removePackage() {
    QString packageName = "hello";
    QSignalSpy spy(m_pkgMgr, &PackageManager::packageRemoved);
    QSignalSpy spyErr(m_pkgMgr, &PackageManager::taskFailed);
    QSignalSpy spyLog(m_pkgMgr, &PackageManager::newTaskData);
    m_pkgMgr->requestPackageRemoval(packageName);
    while (!(spy.count() > 0
             || spy.wait()
             || spyErr.count() > 0
             || spyErr.wait())) {}
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyErr.count(), 0);
    QVERIFY(spyLog.count() >= 0);
}

QTEST_GUILESS_MAIN(TestPackageManager)
#include "TestPackageManager.moc"
