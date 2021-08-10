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

    _pCurrentReply = _pManager->head(_CurrentRequest);
    connect(_pCurrentReply, SIGNAL(finished()), this, SLOT(finishedHead()));
    connect(_pCurrentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

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
    
    if(_pCurrentReply)
        return;
    QList<QByteArray> list = _pCurrentReply->rawHeaderList();
    foreach (QByteArray header, list)
    {
        QString qsLine = QString(header) + " = " + _pCurrentReply->rawHeader(header);
        addLine(qsLine);
    }

    if (_pCurrentReply->hasRawHeader("Accept-Ranges"))
    {
        QString qstrAcceptRanges = _pCurrentReply->rawHeader("Accept-Ranges");
        _bAcceptRanges = (qstrAcceptRanges.compare("bytes", Qt::CaseInsensitive) == 0);
        // qDebug() << "Accept-Ranges = " << qstrAcceptRanges << _bAcceptRanges;
    }

    _nDownloadTotal = _pCurrentReply->header(QNetworkRequest::ContentLengthHeader).toInt();

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
    auto fileSize = _pFile->size();
    _pFile->close();
    QFile::remove(item.localFilePath + item.localFileName);
    if(fileSize)
        _pFile->rename(item.localFilePath + item.localFileName + ".part", item.localFilePath + item.localFileName);
    _pFile = NULL;
    _pCurrentReply = 0;
    emit downloadComplete(item);
}


void FileDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    _Timer.stop();
    _nDownloadSize = _nDownloadSizeAtPause + bytesReceived;
    // qDebug() << "Download Progress: Received=" << _nDownloadSize <<": Total=" << _nDownloadSizeAtPause + bytesTotal;

    _pFile->write(_pCurrentReply->readAll());
    int nPercentage = static_cast<int>((static_cast<float>(_nDownloadSizeAtPause + bytesReceived) * 100.0) / static_cast<float>(_nDownloadSizeAtPause + bytesTotal));
    // qDebug() << nPercentage;
    emit progress(nPercentage);

    _Timer.start(5000);
}


void FileDownloader::error(QNetworkReply::NetworkError code)
{
    emit downloadFailed(item);
}


void FileDownloader::timeout()
{
    qDebug() << __FUNCTION__;
}
