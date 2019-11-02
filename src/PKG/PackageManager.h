//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#ifndef PX_SOFTWARE_PACKAGEMANAGER_H
#define PX_SOFTWARE_PACKAGEMANAGER_H

#include <QObject>
#include <QPointer>
#include "DataAccessLayer.h"
#include "GuixParser.h"
#include "../AsyncTaskRunner.h"

namespace PKG {
    class PackageManager : public QObject {
        Q_OBJECT
    public:
        explicit PackageManager(const QString &dbPath, QObject *parent = nullptr);

    public slots:
        void requestInstalledPackages();
        void requestUserUpgradablePackages();
        void requestSystemUpgradablePackages();

    signals:
        void failed(const QString &message);
        void installedPackagesReady(const QVector<Package *> &packageList);
        void userUpgradablePackagesReady(const QVector<Package *> &packageList);
        void systemUpgradablePackagesReady(const QVector<Package *> &packageList);

    private:
        DataAccessLayer *m_db;
        GuixParser *m_parser;
    };
}

#endif //PX_SOFTWARE_PACKAGEMANAGER_H
