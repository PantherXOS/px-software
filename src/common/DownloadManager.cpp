#include "DownloadManager.h"

DownloadManager *DownloadManager::_instance = nullptr;

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
    ,_downloader(new FileDownloader(parent))
{
    connect(_downloader, &FileDownloader::downloadComplete,[&](const FileDownloader::DownloadItem &item){
        emit downloadComplete(item);
        qDebug() << item.url.toString() << "->" << item.localFilePath + item.localFileName;
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

QUuid DownloadManager::download(FileDownloader::DownloadItem item){
    QUuid id;
    if(!item.url.isValid() && item.url.isEmpty()){
        return id;
    }
    id = QUuid::createUuid();
    item.uuid = id;
    _mLock.lock();
    _downloadQueue.enqueue(item);
    auto size = _downloadQueue.size();
    _mLock.unlock();
    qDebug() << " -> Download Manager(" + QString::number(size) + QString(")") << id.toString();
    return id;
}

void DownloadManager::downloaderRunner(){
    _mLock.lock();
    auto size = _downloadQueue.size();
    if(size){
        auto item = _downloadQueue.dequeue();
        _mLock.unlock();
        qDebug() << " <- Download Manager(" + QString::number(size) + QString(")") << item.uuid.toString();
        _downloader->start(item);
        return;
    }
    _isDownloading = false;
    _mLock.unlock();
}
 