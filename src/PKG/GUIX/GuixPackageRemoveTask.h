//
// Created by Reza Alizadeh Majd on 12/1/19.
//

#ifndef PX_SOFTWARE_GUIXPACKAGEREMOVETASK_H
#define PX_SOFTWARE_GUIXPACKAGEREMOVETASK_H

#include "GuixTask.h"

namespace PKG {
    class GuixPackageRemovalTask : public GuixTask {
        Q_OBJECT
    public:
        explicit GuixPackageRemovalTask(const QString &packageName, QObject *parent = nullptr);

    protected slots:
        void parseWorkerOutput(const QString &outData, const QString &errData) override;

    signals:
        void packageRemoved(const QString &packageName);

    private:
        QString m_packageName;
    };
}


#endif //PX_SOFTWARE_GUIXPACKAGEREMOVETASK_H
