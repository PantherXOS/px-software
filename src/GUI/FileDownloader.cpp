//
// Created by hamzeh on 11/5/19.
//

#include "FileDownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl, QString path, QObject *parent) :
        QObject(parent)
{
    QString localFileName = imageUrl.fileName();
    system((QString("mkdir -p ") + path).toStdString().c_str());
    connect(
            &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*))
    );
    localFilePath = QUrl(path + localFileName);
    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
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
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}