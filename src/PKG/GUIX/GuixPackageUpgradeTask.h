//
// Created by Reza Alizadeh Majd on 12/1/19.
//

#ifndef PX_SOFTWARE_GUIXPACKAGEUPGRADETASK_H
#define PX_SOFTWARE_GUIXPACKAGEUPGRADETASK_H

#include "GuixTask.h"

namespace PKG {
    class GuixPackageUpdateTask : public GuixTask {
        Q_OBJECT
    public:
        explicit GuixPackageUpdateTask(const QStringList &packageList, QObject *parent = nullptr);

    protected slots:
        void parseWorkerOutput(const QString &outData, const QString &errData) override;

    signals:
        void packagesUpdated(const QStringList &packageList);

    private:
        QStringList m_packageList;
    };
}

#endif //PX_SOFTWARE_GUIXPACKAGEUPGRADETASK_H
