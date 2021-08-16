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

#include "OtherApplicationsWidgetItem.h"
#include "Settings.h"

OtherApplicationsWidgetItem_widget::OtherApplicationsWidgetItem_widget(QWidget *parent) : QWidget(parent) {
    QLabel *label = new QLabel("Other Applications");
    auto font = label->font();
    font.setPointSize(OTHER_PACKAGES_FONT_SIZE);
    label->setFont(font);
    label->setContentsMargins(20,20,20,0);

    QLabel *descLabel = new QLabel("<font color='gray'>Applications that do not have a software page with screenshots.</font>");
    auto descFont = descLabel->font();
    descFont.setPointSize(OTHER_PACKAGES_DESC_FONT_SIZE);
    descLabel->setFont(descFont);
    descLabel->setContentsMargins(20,5,20,20);
    // descLabel->setStyleSheet(OTHER_PACKAGE_DESC_STYLESHEET);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(descLabel);
    layout->setSpacing(0);
    layout->setMargin(0);
    setLayout(layout);
    setContentsMargins(0,0,0,0);
    setMouseTracking(false);
}
