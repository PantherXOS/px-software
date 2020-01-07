//
// Created by Reza Alizadeh Majd on 1/5/20.
//

#ifndef PX_SOFTWARE_LXQTSESSIONINTERFACE_H
#define PX_SOFTWARE_LXQTSESSIONINTERFACE_H

#include <QtDBus>

class LxqtSessionInterface : public QObject {
    Q_OBJECT
public:
    explicit LxqtSessionInterface(QObject *parent = nullptr);

protected:
    void changePanelState(bool running);

public:
    void startPanel();
    void stopPanel();
    void restartPanel();

signals:
    void panelStarted();
    void panelStopped();
    void panelRestarted();

protected slots:
    void handleModuleState(const QString&, bool);

protected:
    QDBusInterface *m_interface = nullptr;

};

#endif //PX_SOFTWARE_LXQTSESSIONINTERFACE_H
