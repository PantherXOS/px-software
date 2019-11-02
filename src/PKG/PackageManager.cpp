//
// Created by Reza Alizadeh Majd on 10/21/19.
//

#include "PackageManager.h"

namespace PKG {
    PackageManager::PackageManager(const QString &dbPath, QObject *parent) : QObject(parent) {
        m_db = new DataAccessLayer(dbPath, this);
        m_parser = new GuixParser(m_db);
    }

    void PackageManager::requestInstalledPackages() {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        connect(worker, &AsyncTaskRunner::done, [&](const QString &data) {
            QMap<QString, QString> guixDict;
            for (const auto &line : data.split('\n')) {
                auto params = line.trimmed().split('\t');
                if (params.length() == 4) {
                    QString name = params[0];
                    QString version = params[1];
                    guixDict[name] = version;
                }
            }
            auto packageList = m_db->packageList(guixDict.keys());
            for (auto *pkg : packageList) {
                bool installed = guixDict.contains(pkg->name());
                bool updateAvailable = installed && (guixDict[pkg->name()] != pkg->version());
                pkg->setInstalled(installed);
                pkg->setUpdateAvailable(updateAvailable);
            }
            emit installedPackagesReady(packageList);
            qDeleteAll(packageList);
        });
        connect(worker, &AsyncTaskRunner::failed, this, &PackageManager::failed);
        worker->asyncRun("guix", QStringList() << "package" << "--list-installed");
    }

    void PackageManager::requestUserUpgradablePackages() {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        connect(worker, &AsyncTaskRunner::done, [&](const QString &stdOut, const QString &stdErr) {
            auto pkgList = m_parser->parseUpdateList(stdErr);
            emit userUpgradablePackagesReady(pkgList);
            qDeleteAll(pkgList);
        });
        connect(worker, &AsyncTaskRunner::failed, this, &PackageManager::failed);
        worker->asyncRun("guix", QStringList() << "package"
                                               << "-n"
                                               << "-u");
    }

    void PackageManager::requestSystemUpgradablePackages() {
        QPointer<AsyncTaskRunner> worker = new AsyncTaskRunner(this);
        connect(worker, &AsyncTaskRunner::done, [&](const QString &stdOut, const QString &stdErr) {
            auto pkgList = m_parser->parseUpdateList(stdErr);
            emit systemUpgradablePackagesReady(pkgList);
            qDeleteAll(pkgList);
        });
        connect(worker, &AsyncTaskRunner::failed, this, &PackageManager::failed);
        worker->asyncRun("guix", QStringList() << "package"
                                               << "-p" << "/run/current-system/profile"
                                               << "-n"
                                               << "-u");
    }
}
