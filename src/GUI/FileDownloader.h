//
// Created by hamzeh on 11/5/19.
//

#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

class FileDownloader : public QObject
{
Q_OBJECT
public:
    explicit FileDownloader(QUrl imageUrl, QString path, QObject *parent = 0);
    virtual ~FileDownloader();
    QUrl localFilePath;

signals:
    void downloaded(const QString &localfile);

private slots:
    void fileDownloaded(QNetworkReply* pReply);

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
};

#endif // FILEDOWNLOADER_H