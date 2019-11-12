//
// Created by Reza Alizadeh Majd on 11/10/19.
//

#include "GuixInstalledPackagesTask.h"

namespace PKG {
    GuixInstalledPackagesTask::GuixInstalledPackagesTask(QObject *parent) :
            GuixTask(QStringList() << "package" << "--list-installed", parent) {

    }

    void GuixInstalledPackagesTask::parseWorkerOutput(const QString &outData, const QString &errData) {
        QStringList guixPackages;
        for (const auto &line : outData.split('\n')) {
            auto params = line.trimmed().split('\t');
            if (params.length() == 4) {
                guixPackages << params[0];
            }
        }
        emit packageListReady(guixPackages);
    }
}