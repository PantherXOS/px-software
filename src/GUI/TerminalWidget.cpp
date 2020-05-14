/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(const QString &title, PxQScrollArea *parent) : PxQScrollArea(title,
                                                                                      parent) {
    messageBox = new QLabel(this);
    messageBox->setWordWrap(true);
    messageBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    messageBox->showMaximized();
    messageBox->setAlignment(Qt::AlignTop);
    auto pal = QGuiApplication::palette();
    auto bgcolor = pal.color(QPalette::Active, QPalette::Base);
    auto fgcolor = pal.color(QPalette::Active, QPalette::Text);
    messageBox->setStyleSheet(QString(QLABEL_STYLE_FROM_COLOR_SCHEME).arg(bgcolor.name(),fgcolor.name()));

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
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}
