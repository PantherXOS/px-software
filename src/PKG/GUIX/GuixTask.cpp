//
// Created by Reza Alizadeh Majd on 11/10/19.
//

#include "GuixTask.h"

#include <utility>

namespace PKG {
    GuixTask::GuixTask(QStringList args, QObject *parent) :
            AsyncTaskRunner("guix", std::move(args), parent) {
        connect(this, &AsyncTaskRunner::done, this, &GuixTask::parseWorkerOutput);
    }
}
