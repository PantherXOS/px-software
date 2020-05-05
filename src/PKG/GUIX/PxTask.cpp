//
// Created by Hamzeh Nasajpour on 04/05/20.
//

#include "PxTask.h"

#include <utility>

namespace PKG {
    PxTask::PxTask(QStringList args, QObject *parent) :
            AsyncTaskRunner("px-software-update-check", std::move(args), parent) {
        connect(this, &AsyncTaskRunner::done, this, &PxTask::parseWorkerOutput);
    }
}

