//
// Created by Reza Alizadeh Majd on 11/12/19.
//

#include "GuixProfileStatusTask.h"
#include "GuixInstalledPackagesTask.h"
#include "GuixUpgradablePackagesTask.h"

namespace PKG {
    GuixProfileStatusTask::GuixProfileStatusTask(QObject *parent) :
            GuixTask(QStringList(), parent),
            m_installedWorker(new GuixInstalledPackagesTask(this)),
            m_upgradableWorker(new GuixUpgradablePackagesTask(GuixPackageProfiles::USER, this)) {

        connect(m_installedWorker, &GuixInstalledPackagesTask::packageListReady,
                this, &GuixProfileStatusTask::onInstalledPackagesReady);
        connect(m_installedWorker, &AsyncTaskRunner::newData, this, &AsyncTaskRunner::newData);
        connect(m_installedWorker, &AsyncTaskRunner::failed, this, &AsyncTaskRunner::failed);

        connect(m_upgradableWorker, &GuixUpgradablePackagesTask::packageListReady,
                this, &GuixProfileStatusTask::onUpgradablePackagesReady);
        connect(m_upgradableWorker, &AsyncTaskRunner::newData, this, &AsyncTaskRunner::newData);
        connect(m_upgradableWorker, &AsyncTaskRunner::failed, this, &AsyncTaskRunner::failed);
    }

    bool GuixProfileStatusTask::asyncRun() {
        m_installedPackages.clear();
        m_upgradablePackages.clear();
        return m_installedWorker->asyncRun();
    }

    void GuixProfileStatusTask::onInstalledPackagesReady(const QStringList &installedPackages) {
        m_installedPackages = installedPackages;
        m_upgradableWorker->asyncRun();
    }

    void GuixProfileStatusTask::onUpgradablePackagesReady(const QStringList &upgradablePackages) {
        m_upgradablePackages = upgradablePackages;
        emit packageListReady(m_installedPackages, m_upgradablePackages);
    }
}
