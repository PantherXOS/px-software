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

#include "FileDownloader.h"

FileDownloader::FileDownloader(QObject *parent) :
        QObject(parent) {
}

int FileDownloader::start(QUrl imageUrl, QString path) {
    QString localFileName = imageUrl.fileName();
    localFilePath = QUrl(path + localFileName);
    if(QFile(localFilePath.toString()).exists())
        emit downloaded(localFilePath.toString());
    else {
        system((QString("mkdir -p ") + path).toStdString().c_str());
        connect(
                &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
                this, SLOT (fileDownloaded(QNetworkReply*))
        );
        QNetworkRequest request(imageUrl);
        m_WebCtrl.get(request);
    }
    return 0;
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();
    QFile file(localFilePath.toString());
    file.open(QIODevice::WriteOnly);
    file.write(m_DownloadedData);
    file.close();

    //emit a signal
    pReply->deleteLater();
    emit downloaded(localFilePath.toString());
}