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

class TerminalWidget : public QScrollArea{
Q_OBJECT
public:
    TerminalWidget(const QString &packageName, QScrollArea *parent = nullptr);
    void showMessage(const QString & message);

private:
    QLabel *messageBox;
};


#endif //PX_SOFTWARE_TERMINALWIDGET_H
