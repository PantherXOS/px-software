#include "FileDownloader.h"

#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include <QDir>

FileDownloader::FileDownloader(QObject *parent) :
    QObject(parent)
    , _pManager(NULL)
    , _pCurrentReply(NULL)
    , _pFile(NULL)
    , _nDownloadTotal(0)
    , _bAcceptRanges(false)
    , _nDownloadSize(0)
    , _nDownloadSizeAtPause(0)
    , _timeoutCounter(0)
{
}


FileDownloader::~FileDownloader()
{
    
}


void FileDownloader::start(const DownloadItem &_item)
{
    item = _item;
    if(!item.url.isValid() && item.url.isEmpty()){
        qWarning() << "Invalid URL: " << item.url;
        return;
    }

    if (!item.localFilePath.isEmpty()) {
        QDir().mkpath(item.localFilePath);
    }

    QFileInfo fileInfo(item.url.toString());
    _nDownloadSize = 0;
    _nDownloadSizeAtPause = 0;

    _pManager = new QNetworkAccessManager(this);
    _CurrentRequest = QNetworkRequest(item.url);

    _pCurrentReplyHead = _pManager->head(_CurrentRequest);
    connect(_pCurrentReplyHead, SIGNAL(finished()), this, SLOT(finishedHead()));
    connect(_pCurrentReplyHead, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    _Timer.setInterval(5000);
    _Timer.setSingleShot(true);
    connect(&_Timer, SIGNAL(timeout()), this, SLOT(timeout()));
    _Timer.start();
}

void FileDownloader::download()
{
    if (_bAcceptRanges)
    {
        QByteArray rangeHeaderValue = "bytes=" + QByteArray::number(_nDownloadSizeAtPause) + "-";
        if (_nDownloadTotal > 0)
        {
            rangeHeaderValue += QByteArray::number(_nDownloadTotal);
        }
        _CurrentRequest.setRawHeader("Range", rangeHeaderValue);
    }

    _pCurrentReply = _pManager->get(_CurrentRequest);

    _Timer.setInterval(5000);
    _Timer.setSingleShot(true);
    connect(&_Timer, SIGNAL(timeout()), this, SLOT(timeout()));
    _Timer.start();

    connect(_pCurrentReply, SIGNAL(finished()), this, SLOT(finished()));
    connect(_pCurrentReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(_pCurrentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}


void FileDownloader::finishedHead()
{
    _Timer.stop();
    _bAcceptRanges = false;
    
    if(!_pCurrentReplyHead)
        return;
    QList<QByteArray> list = _pCurrentReplyHead->rawHeaderList();
    foreach (QByteArray header, list)
    {
        QString qsLine = QString(header) + " = " + _pCurrentReplyHead->rawHeader(header);
        addLine(qsLine);
    }

    if (_pCurrentReplyHead->hasRawHeader("Accept-Ranges"))
    {
        QString qstrAcceptRanges = _pCurrentReplyHead->rawHeader("Accept-Ranges");
        _bAcceptRanges = (qstrAcceptRanges.compare("bytes", Qt::CaseInsensitive) == 0);
        // qDebug() << "Accept-Ranges = " << qstrAcceptRanges << _bAcceptRanges;
    }

    _nDownloadTotal = _pCurrentReplyHead->header(QNetworkRequest::ContentLengthHeader).toInt();

//    _CurrentRequest = QNetworkRequest(url);
    _CurrentRequest.setRawHeader("Connection", "Keep-Alive");
    _CurrentRequest.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    _pFile = new QFile(item.localFilePath + item.localFileName + ".part");
    if (!_bAcceptRanges)
    {
        _pFile->remove();
    }
    _pFile->open(QIODevice::ReadWrite | QIODevice::Append);

    _nDownloadSizeAtPause = _pFile->size();
    download();
}


void FileDownloader::finished()
{
    _Timer.stop();
    if(_pFile){
        auto fileSize = _pFile->size();
        _pFile->close();
        QFile::remove(item.localFilePath + item.localFileName);
        if(fileSize) {
            _pFile->rename(item.localFilePath + item.localFileName + ".part", item.localFilePath + item.localFileName);
            emit downloadComplete(item);
        } else {
            emit downloadFailed(item);        
        }
        _pFile = NULL;
    } else {
        emit downloadFailed(item);
    }
    _pCurrentReply = 0;
}


void FileDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    _Timer.stop();
    _nDownloadSize = _nDownloadSizeAtPause + bytesReceived;
    // qDebug() << "Download Progress: Received=" << _nDownloadSize <<": Total=" << _nDownloadSizeAtPause + bytesTotal;

    if(!_pCurrentReply)
        return;
    _pFile->write(_pCurrentReply->readAll());
    int nPercentage = static_cast<int>((static_cast<float>(_nDownloadSizeAtPause + bytesReceived) * 100.0) / static_cast<float>(_nDownloadSizeAtPause + bytesTotal));
    // qDebug() << nPercentage;
    emit progress(nPercentage);

    _Timer.start(5000);
}


void FileDownloader::error(QNetworkReply::NetworkError code)
{
    qWarning() << item.url.toString() << code;
}


void FileDownloader::timeout()
{
    _timeoutCounter++;
    qWarning() << item.url.toString() << ": timeout (" << _timeoutCounter << ")";
    if(_timeoutCounter > 3)
        emit downloadFailed(item);
}
