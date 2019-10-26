//
// Created by Reza Alizadeh Majd on 10/23/19.
//

#include <QtTest/QtTest>
#include <QDebug>
#include <RecDB.h>

class TestRecDB : public QObject {
Q_OBJECT
private slots:
    void getAllRecords();
    void getFilteredRecords();
    void getMultiFilterRecords();
    void getMultiField();
    void loadMultiFile();
private:
    QString m_categoryPath = "./SAMPLE_DB/category.rec";
    QString m_packagePath = "./SAMPLE_DB/packages";
};

void TestRecDB::getAllRecords() {
    RecDB db(m_categoryPath);
    auto recs = db.find();
    QCOMPARE(recs.size(), 5);
}

void TestRecDB::getFilteredRecords() {
    RecDB db(m_categoryPath);
    auto recs = db.find("name ~ 'ent'");
    QCOMPARE(recs.size(), 2);
}

void TestRecDB::getMultiFilterRecords() {
    RecDB db(m_categoryPath);
    auto recs = db.find("(name ~ 'ent') && (description ~ 'devel')");
    QCOMPARE(recs.size(), 1);
}

void TestRecDB::getMultiField() {
    RecDB db(m_packagePath);
    auto recs = db.find();
    QVERIFY(!recs.empty());
    QVERIFY(recs[0].contains("category"));
    QVERIFY(recs[0]["category"].size() > 1);
}

void TestRecDB::loadMultiFile() {
    RecDB db(m_packagePath);
    auto recs = db.find();
    QCOMPARE(recs.size(), 2);
}

QTEST_APPLESS_MAIN(TestRecDB)
#include <TestRecDB.moc>
