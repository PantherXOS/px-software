//
// Created by Reza Alizadeh Majd on 11/10/19.
//

#include "PxTask.h"

#include <utility>

namespace PKG {
    PxTask::PxTask(QStringList args, QObject *parent) :
            AsyncTaskRunner("px-software-update-check", std::move(args), parent) {
        connect(this, &AsyncTaskRunner::done, this, &PxTask::parseWorkerOutput);
    }
}

