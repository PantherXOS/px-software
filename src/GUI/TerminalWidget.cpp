//
// Created by hamzeh on 11/16/19.
//

#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(const int & id, const QString &title, PxQScrollArea *parent) : PxQScrollArea(title,
                                                                                                            parent) {
    messageBox = new QLabel;
    messageBox->setWordWrap(true);
    messageBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    messageBox->showMaximized();
    messageBox->setStyleSheet("QLabel {background-color: black; color: white;}");
    messageBox->setAlignment(Qt::AlignTop);
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->addWidget(messageBox);
    QWidget *widget=new QWidget;
    widget->setLayout(boxLayout);
    widget->showMaximized();
    widget->setStyleSheet("background-color: black;");
    setWidgetResizable(true);
    setWidget(widget);
    messageBox->setText("\n + Embedded Terminal for \"" + title + "\" Log Messages\n\n");
}

void TerminalWidget::showMessage(const QString &message) {
    messageBox->setText(message);
}
