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

#include "DataEntities.h"
#include "DataAccessLayer.h"

namespace PKG {

    BaseEntity::BaseEntity(QObject *parent) : QObject(parent) {}

    bool BaseEntity::checkAndSetProperty(const QString &propertyName,
                                         const RecRecord &record,
                                         bool isList,
                                         const QString &sourceKey) {
        auto srcField = (sourceKey.isEmpty() ? propertyName : sourceKey);
        if (record.contains(srcField)) {
            QVariant value = isList ?
                             QVariant::fromValue(record[srcField]) :
                             QVariant::fromValue(record[srcField][0]);
            return this->setProperty(propertyName.toStdString().c_str(), value);
        }
        return true;
    }
}

namespace PKG {

    Category::Category(QObject *parent) : BaseEntity(parent) {}

    Category *Category::MakeCategory(const RecRecord &rec, QObject *parent) {
        auto *cat = new Category(parent);
        bool isReady = cat->checkAndSetProperty("name", rec)
                       && cat->checkAndSetProperty("title", rec)
                       && cat->checkAndSetProperty("icon", rec)
                       && cat->checkAndSetProperty("description", rec)
                       && !cat->name().isEmpty();
        if (!isReady) {
            cat->deleteLater();
            cat = nullptr;
        }
        return cat;
    }
}


namespace PKG {
    Package::Package(QObject *parent) : BaseEntity(parent) {}

    Package *Package::MakePackage(const RecRecord &rec, QObject *parent) {
        auto *pkg = new Package(parent);
        bool isReady = pkg->checkAndSetProperty("name", rec)
                       && pkg->checkAndSetProperty("title", rec)
                       && pkg->checkAndSetProperty("version", rec)
                       && pkg->checkAndSetProperty("description", rec)
                       && pkg->checkAndSetProperty("homepage", rec)
                       && pkg->checkAndSetProperty("license", rec)
                       && pkg->checkAndSetProperty("icon", rec)
                       && pkg->checkAndSetProperty("size", rec)
                       && pkg->checkAndSetProperty("screenshots", rec, true, "screenshot")
                       && pkg->checkAndSetProperty("categories", rec, true, "category")
                       && pkg->checkAndSetProperty("tags", rec, true, "tag")
                       && !pkg->name().isEmpty();
        if (!isReady) {
            pkg->deleteLater();
            pkg = nullptr;
        }
        return pkg;
    }
}
