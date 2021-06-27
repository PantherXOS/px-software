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
    QString m_categoryPath = QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation)[0] + QString("/px-software-assets/db/category.rec");
    QString m_packagePath =  QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation)[0] + QString("/px-software-assets/db/packages");
};

void TestRecDB::getAllRecords() {
    RecDB db(m_categoryPath);
    auto recs = db.find();
    QVERIFY(recs.size() > 0);
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
    auto recs = db.find("name = 'vim'");
    QVERIFY(!recs.empty());
    QVERIFY(recs[0].contains("category"));
    QVERIFY(recs[0]["category"].size() > 1);
}

void TestRecDB::loadMultiFile() {
    RecDB db(m_packagePath);
    auto recs = db.find();
    QVERIFY(!recs.empty());
}

QTEST_APPLESS_MAIN(TestRecDB)
#include <TestRecDB.moc>
