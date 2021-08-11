#ifndef FILE_DOWNLOADER_H
#define FILE_DOWNLOADER_H

#include <QtGlobal>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QTimer>
#include <QUuid>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    class DownloadItem{
    public:
        DownloadItem(){};
        DownloadItem(const QUrl &url, const QString &path, const QString &name){
            this->url = url;
            this->localFilePath = path;
            this->localFileName = name;
        }
        QUuid   uuid;
        QUrl    url;
        QString localFilePath="";
        QString localFileName="";
    };

    explicit FileDownloader(QObject *parent = 0);
    virtual ~FileDownloader();

signals:
    void addLine(QString qsLine);
    void downloadComplete(const FileDownloader::DownloadItem &item);
    void downloadFailed(const FileDownloader::DownloadItem &item);
    void progress(int nPercentage);

public slots:
    void start(const DownloadItem &item);
    
private slots:
    void download();
    void finishedHead();
    void finished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);
    void timeout();

private:
    DownloadItem item;
    QNetworkAccessManager* _pManager;
    QNetworkRequest _CurrentRequest;
    QNetworkReply* _pCurrentReply, *_pCurrentReplyHead;
    QFile* _pFile;
    int _nDownloadTotal;
    bool _bAcceptRanges;
    int _nDownloadSize;
    int _nDownloadSizeAtPause;
    int _timeoutCounter;
    QTimer _Timer;
};

#endif // FILE_DOWNLOADER_H
