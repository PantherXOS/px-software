//
// Created by Reza Alizadeh Majd on 11/10/19.
//

#ifndef PX_SOFTWARE_GUIXUPGRADABLEPACKAGESTASK_H
#define PX_SOFTWARE_GUIXUPGRADABLEPACKAGESTASK_H

#include "PxTask.h"
#include <QDebug>

namespace PKG {
    class GuixUpgradablePackagesTask : public PxTask {
        Q_OBJECT
    public:
        explicit GuixUpgradablePackagesTask(GuixPackageProfiles profile, QObject *parent = nullptr);

    protected slots:
        void parseWorkerOutput(const QString &outData, const QString &errData) override;

    signals:
        void packageListReady(const QStringList &packageList);

    private:
        GuixPackageProfiles m_profile;
    };
}

#endif //PX_SOFTWARE_GUIXUPGRADABLEPACKAGESTASK_H
