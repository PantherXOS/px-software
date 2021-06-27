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
    QString m_dbPath = QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation)[0] + QString("/px-software-assets/db");
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