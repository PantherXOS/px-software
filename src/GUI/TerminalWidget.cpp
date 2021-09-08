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
int getComplementaryColor(int color) {
    int R = color & 255;
    int G = (color >> 8) & 255;
    int B = (color >> 16) & 255;
    R = 255 - R;
    G = 255 - G;
    B = 255 - B;
    return R + (G << 8) + ( B << 16);
}

TerminalWidget::TerminalWidget(const QString &title, PXContentWidget *parent) : PXContentWidget("Terminal",
                                                                                      parent) {
    messageBox = new QTextEdit(this);
    messageBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    messageBox->showMaximized();
    messageBox->setReadOnly(true);
    messageBox->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    
    auto layout = new QHBoxLayout;
    layout->addWidget(messageBox);
    layout->setContentsMargins(10,10,10,10);
    
    setLayout(layout);
    setWidgetResizable(true);
    auto fgColor = QGuiApplication::palette().color(QPalette::Active, QPalette::WindowText);
    auto fgColorNum = fgColor.red() + (fgColor.green() << 8) + (fgColor.blue() << 16);
    auto bgColor = QColor(getComplementaryColor(fgColorNum));
    setStyleSheet(QString("QWidget {background-color: %1; color: %2; border: 0px}").arg(bgColor.name(), fgColor.name()));
    
    messageBox->setText("This application is awaiting it's turn. A detailed progress will be shown once installation or removal has started.");
}

void TerminalWidget::showMessage(const QString &message) {
    messageBox->setPlainText(message);
    messageBox->verticalScrollBar()->setSliderPosition(messageBox->verticalScrollBar()->maximum());
}
