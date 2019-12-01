//
// Created by Reza Alizadeh Majd on 12/1/19.
//

#ifndef PX_SOFTWARE_GUIXPACKAGEINSTALLTASK_H
#define PX_SOFTWARE_GUIXPACKAGEINSTALLTASK_H

#include "GuixTask.h"

namespace PKG {
    class GuixPackageInstallTask : public GuixTask {
        Q_OBJECT
    public:
        explicit GuixPackageInstallTask(const QString &packageName, QObject *parent = nullptr);

    protected slots:
        void parseWorkerOutput(const QString &outData, const QString &errData) override;

    signals:
        void packageInstalled(const QString &packageName);

    private:
        QString m_packageName;
    };
}

#endif //PX_SOFTWARE_GUIXPACKAGEINSTALLTASK_H
