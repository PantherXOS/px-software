//
// Created by hamzeh on 11/16/19.
//

#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(const QString &packageName) {
    messageBox = new QLabel;
    messageBox->setWordWrap(true);
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    boxLayout->addWidget(messageBox);
    QWidget *widget=new QWidget;
    widget->setLayout(boxLayout);
    setWidgetResizable(true);
    setWidget(widget);
    show();
}

void TerminalWidget::showMessage(const QString &message) {
    messageBox->setText(messageBox->text()+message);
}
