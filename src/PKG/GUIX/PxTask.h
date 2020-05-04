//
// Created by Reza Alizadeh Majd on 11/10/19.
//

#ifndef PX_SOFTWARE_PXTASK_H
#define PX_SOFTWARE_PXTASK_H

#include "src/AsyncTaskRunner.h"

namespace PKG {

    enum class GuixPackageProfiles {
        USER,
        SYSTEM
    };

    class PxTask : public AsyncTaskRunner {
        Q_OBJECT
    public:
        explicit PxTask(QStringList args, QObject *parent = nullptr);

    protected slots:
        virtual void parseWorkerOutput(const QString &outData, const QString &errData) = 0;
    };
}

#endif //PX_SOFTWARE_PXTASK_H
