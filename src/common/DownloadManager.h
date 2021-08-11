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
    QUuid download(FileDownloader::DownloadItem item);

private slots:
    void downloaderRunner();

private:
    explicit DownloadManager(QObject *parent = 0);

signals:
    void goNextDownload();
    void downloadComplete(const FileDownloader::DownloadItem &item);
    void downloadFailed(const FileDownloader::DownloadItem &item);

private:
    static DownloadManager     *_instance;
    FileDownloader      *_downloader = nullptr;
    QThread             *_thread;
    QMutex               _mLock;
    bool                 _isDownloading = false;
    QObject              _context;
    QQueue<FileDownloader::DownloadItem> _downloadQueue;
};

#endif // DOWNLOADMANAGER_H
