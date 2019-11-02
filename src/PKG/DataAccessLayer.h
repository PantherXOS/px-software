//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#ifndef PX_SOFTWARE_DATAACCESSLAYER_H
#define PX_SOFTWARE_DATAACCESSLAYER_H

#include <QObject>
#include <QVector>
#include "../RecDB.h"
#include "DataEntities.h"

namespace PKG {

    class SearchQueryBase: public QObject {
        Q_OBJECT
    protected:
        explicit SearchQueryBase() = default;
    public:
        virtual QString query() const = 0;
    };

    class TextSearchQuery : public SearchQueryBase {
        Q_OBJECT
    public:
        explicit TextSearchQuery(QString keyword);
        QString query() const override;
    private:
        QString m_keyword;
    };

    class PackageNameSearchQuery: public SearchQueryBase {
        Q_OBJECT
    public:
        explicit PackageNameSearchQuery(QString pkgName);
        QString query() const override;
    private:
        QString m_packageName;
    };

    class PackageListSearchQuery: public SearchQueryBase {
        Q_OBJECT
    public:
        explicit PackageListSearchQuery(QStringList packageNames);
        QString query() const override;
    private:
        QStringList m_packageNames;
    };

    class CategorySearchQuery : public SearchQueryBase {
        Q_OBJECT
    public:
        explicit CategorySearchQuery(QString categoryName);
        QString query() const override;
    private:
        QString m_category;
    };

    class DataAccessLayer : public QObject {
        Q_OBJECT
    public:
        explicit DataAccessLayer(const QString &dbBasePath);

        QVector<Category *> categoryList();
        QVector<Package *> categoryPackages(const QString &categoryName);
        QVector<Package *> findPackages(const QString &keyword);
        QVector<Package *> packageList(const QStringList &packageNames);
        Package *packageDetails(const QString &packageName);

    protected:
        QVector<Package *> performPackageSearch(const QVector<SearchQueryBase*> &filters);

    private:
        RecDB m_categoryDB;
        RecDB m_packageDB;

    };
}

#endif //PX_SOFTWARE_DATAACCESSLAYER_H
