//
// Created by hamzeh on 1/14/20.
//

#ifndef PX_SOFTWARE_PXWINDOWMANAGER_H
#define PX_SOFTWARE_PXWINDOWMANAGER_H

#include <QObject>

#include "PXContentWidget.h"
#include "PXWidget.h"

class PXWindowManager : public QObject {
    Q_OBJECT
public:
    static PXWindowManager *instance();
    void add(const PXContentWidget *widget);

signals:
    void addPxWindow(const PXContentWidget *widget);

private:
    PXWindowManager(QObject *parent = nullptr);
    static PXWindowManager *_instance;
};


#endif //PX_SOFTWARE_PXWINDOWMANAGER_H
