//
// Created by hamzeh on 11/5/19.
//

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