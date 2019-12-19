//
// Created by hamzeh on 11/16/19.
//

#ifndef PX_SOFTWARE_TERMINALWIDGET_H
#define PX_SOFTWARE_TERMINALWIDGET_H

#include <QScrollArea>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QBoxLayout>
#include <QScrollBar>

#include "PxQScrollArea.h"

class TerminalWidget : public PxQScrollArea{
Q_OBJECT
public:
    TerminalWidget(const QString &title, PxQScrollArea *parent = nullptr);
    void showMessage(const QString & message);
private:
    QLabel *messageBox;
};

#endif //PX_SOFTWARE_TERMINALWIDGET_H
