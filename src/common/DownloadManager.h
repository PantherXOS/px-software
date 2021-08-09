#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QUuid>
#include <QMap>
#include <QQueue>
#include <QThread>
#include <QEventLoop>
#include <QMutex>
#include <QMutexLocker>

#include "FileDownloader.h"

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    static DownloadManager *Instance();
    virtual ~DownloadManager();
    QUuid download(const QUrl &url, const QString &savedPath);

private slots:
    void downloaderRunner();

private:
    explicit DownloadManager(QObject *parent = 0);

signals:
    void goNextDownload();
    void downloadComplete(const QUuid &uuid);
    void downloadFailed(const QUuid &uuid);

private:
    class DownloadItem{
    public:
        QUuid   uuid;
        QUrl    url;
        QString savedPath="";
    };
    static DownloadManager     *_instance;
    FileDownloader      *_downloader;
    QQueue<DownloadItem> _downloadQueue;
    QThread             *_thread;
    QMutex               _mLock;
    bool                 _isDownloading = false;
    QObject              _context;
};

#endif // DOWNLOADMANAGER_H
