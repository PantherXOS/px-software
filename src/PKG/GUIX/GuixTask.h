//
// Created by Reza Alizadeh Majd on 11/10/19.
//

#ifndef PX_SOFTWARE_GUIXTASK_H
#define PX_SOFTWARE_GUIXTASK_H

#include "src/AsyncTaskRunner.h"

namespace PKG {

    class GuixTask : public AsyncTaskRunner {
        Q_OBJECT
    public:
        explicit GuixTask(QStringList args, QObject *parent = nullptr);

    protected slots:
        virtual void parseWorkerOutput(const QString &outData, const QString &errData) = 0;
    };
}

#endif //PX_SOFTWARE_GUIXTASK_H
