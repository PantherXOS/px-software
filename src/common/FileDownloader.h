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
    explicit FileDownloader(QObject *parent = 0);
    virtual ~FileDownloader();

signals:
    void addLine(QString qsLine);
    void downloadComplete(const QUuid &uuid, const QString &localfile);
    void progress(int nPercentage);

public slots:
    void start(QUrl url, const QString &savedPath);
    void start(QUrl url, const QUuid &uuid, const QString &savedPath);
    
private slots:
    void download();
    void finishedHead();
    void finished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);
    void timeout();

private:
    QUrl _URL;
    QUuid _uuid;
    QString _qsFileName;
    QNetworkAccessManager* _pManager;
    QNetworkRequest _CurrentRequest;
    QNetworkReply* _pCurrentReply;
    QFile* _pFile;
    int _nDownloadTotal;
    bool _bAcceptRanges;
    int _nDownloadSize;
    int _nDownloadSizeAtPause;
    QTimer _Timer;
};

#endif // FILE_DOWNLOADER_H
