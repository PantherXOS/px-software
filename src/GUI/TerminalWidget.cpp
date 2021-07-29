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
TerminalWidget::TerminalWidget(const QString &title, PXContentWidget *parent) : PXContentWidget(title,
                                                                                      parent) {
    messageBox = new QTextEdit(this);
    messageBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    messageBox->showMaximized();
    messageBox->setReadOnly(true);
    messageBox->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    setWidget(messageBox);
    setWidgetResizable(true);
    
    messageBox->setText("\n + Embedded Terminal for \"" + title + "\" Log Messages\n\n");
}

void TerminalWidget::showMessage(const QString &message) {
    messageBox->setPlainText(message);
    messageBox->verticalScrollBar()->setSliderPosition(messageBox->verticalScrollBar()->maximum());
}
