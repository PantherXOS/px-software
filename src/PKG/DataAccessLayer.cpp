//
// Created by Reza Alizadeh Majd on 10/21/19.
//

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

// DataAccessLayer
namespace PKG {
    DataAccessLayer::DataAccessLayer(const QString &dbBasePath) :
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
         query.append(new PackageListSearchQuery(packageNames));
         auto result = this->performPackageSearch(query);
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

}
