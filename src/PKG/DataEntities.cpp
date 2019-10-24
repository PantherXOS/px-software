//
// Created by Reza Alizadeh Majd on 10/23/19.
//

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
                       && pkg->checkAndSetProperty("screenshots", rec, true, "screenshot")
                       && pkg->checkAndSetProperty("categories", rec, true, "category")
                       && !pkg->name().isEmpty();
        if (!isReady) {
            pkg->deleteLater();
            pkg = nullptr;
        }
        return pkg;
    }
}
