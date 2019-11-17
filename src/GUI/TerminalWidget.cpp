//
// Created by hamzeh on 11/16/19.
//

#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(const QString &packageName, QScrollArea *parent) : QScrollArea(parent) {
    messageBox = new QLabel;
    messageBox->setWordWrap(true);
    messageBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    messageBox->showMaximized();
    messageBox->setStyleSheet("QLabel {background-color: black; color: white;}");
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    boxLayout->addWidget(messageBox);
    QWidget *widget=new QWidget;
    widget->setLayout(boxLayout);
    widget->showMaximized();
    setWidgetResizable(true);
    setWidget(widget);
    showMaximized();
    show();
    messageBox->setText("\n + Embedded Terminal for \"" + packageName + "\" Log Messages\n\n");
}

void TerminalWidget::showMessage(const QString &message) {
    messageBox->setText("\n" + messageBox->text()+ " " + message);
}
