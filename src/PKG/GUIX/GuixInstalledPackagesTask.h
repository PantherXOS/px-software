//
// Created by Reza Alizadeh Majd on 11/10/19.
//

#ifndef PX_SOFTWARE_GUIXINSTALLEDPACKAGESTASK_H
#define PX_SOFTWARE_GUIXINSTALLEDPACKAGESTASK_H

#include "GuixTask.h"

namespace PKG {
    class GuixInstalledPackagesTask : public GuixTask {
        Q_OBJECT
    public:
        explicit GuixInstalledPackagesTask(QObject *parent = nullptr);

    protected slots:
        void parseWorkerOutput(const QString &outData, const QString &errData) override;

    signals:
        void packageListReady(const QStringList &packageList);
    };
}

#endif //PX_SOFTWARE_GUIXINSTALLEDPACKAGESTASK_H
