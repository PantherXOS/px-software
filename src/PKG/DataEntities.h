//
// Created by Reza Alizadeh Majd on 10/23/19.
//

#ifndef PX_SOFTWARE_DATAENTITIES_H
#define PX_SOFTWARE_DATAENTITIES_H

#include <QObject>
#include <QVariant>
#include "../RecDB.h"

namespace PKG {

    class DataAccessLayer;

    class BaseEntity : public QObject {
        Q_OBJECT
    public:
        explicit BaseEntity(QObject *parent = nullptr);

        bool checkAndSetProperty(const QString &propertyName,
                                 const RecRecord &record,
                                 bool isList = false,
                                 const QString &sourceKey = QString());
    };


    class Category : public BaseEntity {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(QString icon READ icon WRITE setIcon)
        Q_PROPERTY(QString description READ description WRITE setDescription)
    protected:
        explicit Category(QObject *parent = nullptr);

    public:
        static Category* MakeCategory(const RecRecord &rec, QObject *parent);

        QString name() const { return m_name; }
        void setName(const QString &value) { m_name = value; }

        QString title() const { return m_title; }
        void setTitle(const QString &value) { m_title = value; }

        QString icon() const { return m_icon; }
        void setIcon(const QString &value) { m_icon = value; }

        QString description() const { return m_description; }
        void setDescription(const QString &value) { m_description = value; }

    private:
        QString m_name;
        QString m_title;
        QString m_icon;
        QString m_description;
    };


    class Package : public BaseEntity {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(QString version READ version WRITE setVersion)
        Q_PROPERTY(QString description READ description WRITE setDescription)
        Q_PROPERTY(QString homepage READ homePage WRITE setHomePage)
        Q_PROPERTY(QString license READ license WRITE setLicense)
        Q_PROPERTY(QString icon READ icon WRITE setIcon)
        Q_PROPERTY(QStringList screenshots READ screenShots WRITE setScreenShots)
        Q_PROPERTY(QStringList categories READ categories WRITE setCategories)
    protected:
        explicit Package(QObject *parent);

    public:
        static Package* MakePackage(const RecRecord &rec, QObject *parent);

        QString name() const { return m_name; }
        void setName(const QString &value) { m_name = value; }

        QString title() const { return m_title; }
        void setTitle(const QString &value) { m_title = value; }

        QString version() const { return m_version; }
        void setVersion(const QString &value) { m_version = value; }

        QString description() const { return m_description; }
        void setDescription(const QString &value) { m_description = value; }

        QString homePage() const { return m_homePage; }
        void setHomePage(const QString &value) { m_homePage = value; }

        QString license() const { return m_license; }
        void setLicense(const QString &value) { m_license = value; }

        QString icon() const { return m_icon; }
        void setIcon(const QString &value) { m_icon = value; }

        QStringList screenShots() const { return m_screenshots; }
        void setScreenShots(const QStringList &value) { m_screenshots = value; }

        QStringList categories() const { return m_categories; }
        void setCategories(const QStringList &value) { m_categories = value; }

        bool isInstalled() const { return m_installed; }
        void setInstalled(bool value) { m_installed = value; }

        bool isUpdateAvailable() const { return m_updateAvailable; }
        void setUpdateAvailable(bool value) { m_updateAvailable = value; }

    private:
        QString m_name;
        QString m_title;
        QString m_version;
        QString m_description;
        QString m_homePage;
        QString m_license;

        QString m_icon;
        QStringList m_screenshots;
        QStringList m_categories;

        bool m_installed = false;
        bool m_updateAvailable = false;
    };
}

#endif //PX_SOFTWARE_DATAENTITIES_H
