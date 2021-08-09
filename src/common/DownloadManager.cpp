#include "DownloadManager.h"

DownloadManager *DownloadManager::_instance = nullptr;

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
    ,_downloader(new FileDownloader(parent))
{
    connect(_downloader, &FileDownloader::downloadComplete,[&](const QUuid &uuid,const QString &path){
        emit downloadComplete(uuid);
        emit goNextDownload();
    });

    connect(this,SIGNAL(goNextDownload()),this,SLOT(downloaderRunner()));

    _thread = new QThread(this);
    _context.moveToThread(_thread);
    connect(_thread, &QThread::started, &_context, [&](){
        qDebug() << "Download Manager Started.";
        while(true){
            while(true){
                _mLock.lock();
                if(!_downloadQueue.isEmpty() && !_isDownloading){
                    _mLock.unlock();
                    break;
                } else {
                    _mLock.unlock();
                    QThread::msleep(1000);
                }
            }
            _isDownloading = true;
            emit goNextDownload();
        }
    });
    _thread->start();
}

DownloadManager *DownloadManager::Instance(){
    if(_instance == nullptr)
        _instance = new DownloadManager;
    return _instance;
}

DownloadManager::~DownloadManager()
{
    _thread->terminate();
}

QUuid DownloadManager::download(const QUrl &url, const QString &savedPath){
    QUuid id;
    if(!url.isValid() && url.isEmpty()){
        return id;
    }
    id = QUuid::createUuid();
    DownloadItem downloadItem;
    downloadItem.uuid = id;
    downloadItem.url = url;
    downloadItem.savedPath = savedPath;
    _mLock.lock();
    _downloadQueue.enqueue(downloadItem);
    auto size = _downloadQueue.size();
    _mLock.unlock();
    qDebug() << " -> PUSH to Download Manager Queue:" << size;
    return id;
}

void DownloadManager::downloaderRunner(){
    _mLock.lock();
    auto size = _downloadQueue.size();
    if(size){
        auto item = _downloadQueue.dequeue();
        _mLock.unlock();
        qDebug() << " <- POP from Download Manager Queue:" << size;
        _downloader->start(item.url, item.uuid, item.savedPath);
        return;
    }
    _isDownloading = false;
    _mLock.unlock();
}
 