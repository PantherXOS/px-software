//
// Created by hamzeh on 11/16/19.
//

#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(const QString &title, PxQScrollArea *parent) : PxQScrollArea(title,
                                                                                      parent) {
    messageBox = new QLabel(this);
    messageBox->setWordWrap(true);
    messageBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    messageBox->showMaximized();
    messageBox->setAlignment(Qt::AlignTop);
    auto boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->addWidget(messageBox);
    QWidget *widget=new QWidget(this);
    widget->setLayout(boxLayout);
    widget->showMaximized();
    setWidgetResizable(true);
    setWidget(widget);
    messageBox->setText("\n + Embedded Terminal for \"" + title + "\" Log Messages\n\n");
}

void TerminalWidget::showMessage(const QString &message) {
    messageBox->setText(message);
}
