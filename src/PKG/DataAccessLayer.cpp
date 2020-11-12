/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
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

#include "DataAccessLayer.h"

#include <QDir>
#include <QDebug>
#include <utility>

// TextSearchQuery
namespace PKG {
    TextSearchQuery::TextSearchQuery(QString keyword) : m_keyword(std::move(keyword)) {}

    QString TextSearchQuery::query() const {
        return QString("(title ~ '%1' || description ~ '%1')")
                .arg(m_keyword);
    }
}

// PackageNameSearchQuery
namespace PKG {
    PackageNameSearchQuery::PackageNameSearchQuery(QString pkgName) : m_packageName(std::move(pkgName)) {}

    QString PackageNameSearchQuery::query() const {
        return QString("(name = '%1')").arg(m_packageName);
    }
}

// PackageListSearchQuery
namespace PKG {
    PackageListSearchQuery::PackageListSearchQuery(QStringList packageNames) : m_packageNames(std::move(packageNames)) {
    }

    QString PackageListSearchQuery::query() const {
        QString query;
        for (const auto &pkg : m_packageNames) {
            if (!query.isEmpty()) {
                query += " || ";
            }
            query += QString("(name = '%1')").arg(pkg);
        }
        return QString("(%1)").arg(query);
    }
}

// CategorySearchQuery
namespace PKG {
    CategorySearchQuery::CategorySearchQuery(QString categoryName) : m_category(std::move(categoryName)) {}

    QString CategorySearchQuery::query() const {
        return QString("(category = '%1')")
                .arg(m_category);
    }
}

// TagSearchQuery
namespace PKG {
    TagSearchQuery::TagSearchQuery(QStringList tagNames) : m_tags(std::move(tagNames)) {
    }

    QString TagSearchQuery::query() const {
        QString query;
        for (const QString &tag : m_tags) {
            query += QString(" %1 (tag = '%2')")
                    .arg(query.isEmpty() ? "" : "||")
                    .arg(tag);
        }
        return QString("(%1)").arg(query);
    }
}

// DataAccessLayer
namespace PKG {
    DataAccessLayer::DataAccessLayer(const QString &dbBasePath, QObject *parent) :
            QObject(parent),
            m_categoryDB(QString("%1/category.rec").arg(dbBasePath)),
            m_packageDB(QString("%1/packages/").arg(dbBasePath)) {}

    QVector<Category *> DataAccessLayer::categoryList() {
        QVector<Category *> result;
        auto rawRecords = m_categoryDB.find();
        for (const auto &rec : rawRecords) {
            auto *cat = Category::MakeCategory(rec, this);
            if (cat != nullptr) {
                result.append(cat);
            }
        }
        return result;
    }

    QVector<Package *> DataAccessLayer::findPackages(const QString &keyword) {
        QVector<SearchQueryBase *> query;
        query.append(new TextSearchQuery(keyword));
        auto result = this->performPackageSearch(query);
        qDeleteAll(query);
        return result;
    }

    QVector<Package *> DataAccessLayer::packageList(const QStringList &packageNames) {
        QVector<SearchQueryBase *> query;
        QVector<Package *> result;
        if(packageNames.isEmpty())
            return result;
        query.append(new PackageListSearchQuery(packageNames));
        result = this->performPackageSearch(query);
        qDeleteAll(query);
        return result;
    }

    QVector<Package *> DataAccessLayer::categoryPackages(const QString &categoryName) {
        QVector<SearchQueryBase *> query;
        query.append(new CategorySearchQuery(categoryName));
        auto result = this->performPackageSearch(query);
        qDeleteAll(query);
        return result;
    }

    QVector<Package *> DataAccessLayer::tagPackages(const QString &tagName) {
        QVector<SearchQueryBase *> query;
        query.append(new TagSearchQuery(QStringList() << tagName));
        auto result = this->performPackageSearch(query);
        qDeleteAll(query);
        return result;
    }

    Package *DataAccessLayer::packageDetails(const QString &packageName) {
        QVector<SearchQueryBase *> query;
        query.append(new PackageNameSearchQuery(packageName));
        auto result = this->performPackageSearch(query);
        qDeleteAll(query);
        return result.empty() ? nullptr : result[0];
    }

    QVector<Package *> DataAccessLayer::performPackageSearch(const QVector<SearchQueryBase *> &filters) {
        QVector<Package *> result;
        QString queryStr;
        for (const auto &filter : filters) {
            if (!queryStr.isEmpty()) {
                queryStr.append(" && ");
            }
            queryStr.append(filter->query());
        }
        auto recs = m_packageDB.find(queryStr);
        for (const auto &rec : recs) {
            auto *pkg = Package::MakePackage(rec, this);
            if (pkg != nullptr) {
                result.append(pkg);
            }
        }
        return result;
    }

    bool DataAccessLayer::isInitted(){
        if(m_categoryDB.isInitted() && m_packageDB.isInitted())
            return true;
        return false;
    }
}
