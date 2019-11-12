//
// Created by Reza Alizadeh Majd on 11/12/19.
//

#include "GuixCategoryPackagesTask.h"
#include "GuixInstalledPackagesTask.h"
#include "GuixUpgradablePackagesTask.h"

namespace PKG {
    GuixCategoryPackagesTask::GuixCategoryPackagesTask(QObject *parent) :
            GuixTask(QStringList(), parent),
            m_installedWorker(new GuixInstalledPackagesTask(this)),
            m_upgradableWorker(new GuixUpgradablePackagesTask(GuixPackageProfiles::USER, this)) {

        connect(m_installedWorker, &GuixInstalledPackagesTask::packageListReady,
                this, &GuixCategoryPackagesTask::onInstalledPackagesReady);
        connect(m_installedWorker, &AsyncTaskRunner::newData, this, &AsyncTaskRunner::newData);
        connect(m_installedWorker, &AsyncTaskRunner::failed, this, &AsyncTaskRunner::failed);

        connect(m_upgradableWorker, &GuixUpgradablePackagesTask::packageListReady,
                this, &GuixCategoryPackagesTask::onUpgradablePackagesReady);
        connect(m_upgradableWorker, &AsyncTaskRunner::newData, this, &AsyncTaskRunner::newData);
        connect(m_upgradableWorker, &AsyncTaskRunner::failed, this, &AsyncTaskRunner::failed);
    }

    bool GuixCategoryPackagesTask::asyncRun() {
        m_installedPackages.clear();
        m_upgradablePackages.clear();
        return m_installedWorker->asyncRun();
    }

    void GuixCategoryPackagesTask::onInstalledPackagesReady(const QStringList &installedPackages) {
        m_installedPackages = installedPackages;
        m_upgradableWorker->asyncRun();
    }

    void GuixCategoryPackagesTask::onUpgradablePackagesReady(const QStringList &upgradablePackages) {
        m_upgradablePackages = upgradablePackages;
        emit packageListReady(m_installedPackages, m_upgradablePackages);
    }
}
