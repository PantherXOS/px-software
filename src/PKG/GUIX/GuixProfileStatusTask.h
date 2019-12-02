//
// Created by Reza Alizadeh Majd on 11/12/19.
//

#ifndef PX_SOFTWARE_GUIXPROFILESTATUSTASK_H
#define PX_SOFTWARE_GUIXPROFILESTATUSTASK_H

#include "GuixTask.h"

namespace PKG {
    class GuixInstalledPackagesTask;
    class GuixUpgradablePackagesTask;

    struct GuixProfile {
        QStringList installedPackages;
        QStringList upgradablePackages;
        bool refreshed = false;

        void clear() {
            installedPackages.clear();
            upgradablePackages.clear();
            refreshed = false;
        }
    };

    class GuixProfileStatusTask: public GuixTask {
        Q_OBJECT
    public:
        explicit GuixProfileStatusTask(QObject *parent = nullptr);
        bool asyncRun() override;

    protected slots:
        void parseWorkerOutput(const QString &outData, const QString &errData) override {};
        void onInstalledPackagesReady(const QStringList &installedPackages);
        void onUpgradablePackagesReady(const QStringList &upgradablePackages);

    signals:
        void profileIsReady(const GuixProfile &profile);

    private:
        GuixInstalledPackagesTask *m_installedWorker;
        GuixUpgradablePackagesTask *m_upgradableWorker;
        GuixProfile m_profile;
    };
}

#endif //PX_SOFTWARE_GUIXPROFILESTATUSTASK_H
