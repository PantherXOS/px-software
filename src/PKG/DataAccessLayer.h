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

    class TagSearchQuery : public SearchQueryBase {
        Q_OBJECT
    public:
        explicit TagSearchQuery(QStringList tagNames);
        QString query() const override;
    private:
        QStringList m_tags;
    };

    class DataAccessLayer : public QObject {
        Q_OBJECT
    public:
        explicit DataAccessLayer(const QString &dbBasePath, QObject *parent = nullptr);

        QVector<Category *> categoryList();
        QVector<Package *> categoryPackages(const QString &categoryName);
        QVector<Package *> tagPackages(const QString &tagName);
        QVector<Package *> findPackages(const QString &keyword);
        QVector<Package *> packageList(const QStringList &packageNames);
        Package *packageDetails(const QString &packageName);
        bool isInitted();
        
    protected:
        QVector<Package *> performPackageSearch(const QVector<SearchQueryBase*> &filters);

    private:
        RecDB m_categoryDB;
        RecDB m_packageDB;

    };
}

#endif //PX_SOFTWARE_DATAACCESSLAYER_H
