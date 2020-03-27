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
#include <QDir>

class FileDownloader : public QObject
{
Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = nullptr);
    virtual ~FileDownloader();
    int start(QUrl imageUrl, QString path);

signals:
    void downloaded(const QString &localfile);

private slots:
    void fileDownloaded(QNetworkReply* pReply);

private:
    QUrl localFilePath;
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
};

#endif // FILEDOWNLOADER_H