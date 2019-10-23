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

private:
    QString m_dbPath = "./SAMPLE_DB/category.rec";
};

void TestRecDB::getAllRecords() {
    RecDB db(m_dbPath);
    auto recs = db.find("");
    QCOMPARE(recs.size(), 5);
}

void TestRecDB::getFilteredRecords() {
    RecDB db(m_dbPath);
    auto recs = db.find("name ~ 'ent'");
    QCOMPARE(recs.size(), 2);
}

QTEST_APPLESS_MAIN(TestRecDB)
#include <TestRecDB.moc>
