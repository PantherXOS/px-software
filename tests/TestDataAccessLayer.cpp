//
// Created by Reza Alizadeh Majd on 10/22/19.
//

#include <QtTest/QtTest>
#include <QDebug>
#include <PKG/DataAccessLayer.h>

class TestDataAccessLayer : public QObject {
    Q_OBJECT
private slots:
    void getCategories();
    void simpleFind();
    void categoryPackages();
    void tagSearch();
    void getPackage();

private:
    QString m_dbPath = "./SAMPLE_DB";
};

void TestDataAccessLayer::getCategories() {
    PKG::DataAccessLayer dbLayer(m_dbPath);
    auto cats = dbLayer.categoryList();
    QCOMPARE(cats.size(), 5);
    qDeleteAll(cats);
}

void TestDataAccessLayer::simpleFind() {
    PKG::DataAccessLayer dbLayer(m_dbPath);
    auto packages = dbLayer.findPackages("editor");
    QVERIFY(!packages.isEmpty());
    qDeleteAll(packages);
}

void TestDataAccessLayer::categoryPackages() {
    PKG::DataAccessLayer dbLayer(m_dbPath);
    auto packages = dbLayer.categoryPackages("development");
    QVERIFY(!packages.isEmpty());
    qDeleteAll(packages);
}

void TestDataAccessLayer::tagSearch() {
    PKG::DataAccessLayer dbLayer(m_dbPath);
    auto packages = dbLayer.tagPackages("recommended");
    QVERIFY(!packages.isEmpty());
    qDeleteAll(packages);
}

void TestDataAccessLayer::getPackage() {
    PKG::DataAccessLayer dbLayer(m_dbPath);
    auto *pkg = dbLayer.packageDetails("vim");
    QVERIFY(pkg != nullptr);
    QCOMPARE(pkg->name(), QString("vim"));
    QCOMPARE(pkg->title(), QString("Text Editor Based On Vi"));
    QCOMPARE(pkg->version(), QString("8.1.0551"));
    QVERIFY(!pkg->description().isEmpty());
    QCOMPARE(pkg->homePage(), QString("http://www.vim.org/"));
    QCOMPARE(pkg->license(), QString("Vim"));
    QVERIFY(!pkg->icon().isEmpty());
    QCOMPARE(pkg->screenShots().size(), 3);
    QCOMPARE(pkg->categories().size(), 2);
    pkg->deleteLater();
}


QTEST_GUILESS_MAIN(TestDataAccessLayer)
#include "TestDataAccessLayer.moc"