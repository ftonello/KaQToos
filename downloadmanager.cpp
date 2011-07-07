#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QProgressDialog>
#include <QBuffer>

#include <QDebug>

#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent)
	: QObject(parent),
	  httpRequestAborted(false)
{
	progressDialog = new QProgressDialog(qobject_cast<QWidget *>(parent));

	connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
}

DownloadManager::~DownloadManager()
{
	delete progressDialog;

	// buffer will be deleted by the client
}

void DownloadManager::append(const QStringList &urlList)
{
	foreach (QString url, urlList)
		append(QUrl::fromEncoded(url.toLocal8Bit()));

	if (downloadQueue.isEmpty())
		QTimer::singleShot(0, this, SIGNAL(finished()));
}

void DownloadManager::append(const QUrl &url)
{
	if (downloadQueue.isEmpty())
		QTimer::singleShot(0, this, SLOT(startNextDownload()));

	downloadQueue.enqueue(url);
}

void DownloadManager::startNextDownload()
{
	if (downloadQueue.isEmpty()) {
		emit finished();
		return;
	}

	QUrl url = downloadQueue.dequeue();
	currentUrl = url.toString();

	buffer = new QBuffer();
	if (!buffer->open(QBuffer::ReadWrite)) {
		startNextDownload();
		return;                 // skip this download
	}

	progressDialog->show();
	httpRequestAborted = false;

	QNetworkRequest request(url);
	currentDownload = manager.get(request);
	connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
	        SLOT(downloadProgress(qint64,qint64)));
	connect(currentDownload, SIGNAL(finished()),
	        SLOT(downloadFinished()));
	connect(currentDownload, SIGNAL(readyRead()),
	        SLOT(downloadReadyRead()));
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if (httpRequestAborted)
		return;

	progressDialog->setMaximum(bytesTotal);
	progressDialog->setValue(bytesReceived);
}

void DownloadManager::downloadFinished()
{
	progressDialog->reset();
	//buffer->close();

	if (currentDownload->error()) {
		// download failed
		qCritical() << "Failed: " << currentDownload->errorString();
		delete buffer;
		buffer = 0;
	} else {
		buffer->seek(0);
		// if it's ok!
		emit getDownload(buffer, currentUrl);
	}

	currentDownload->deleteLater();
	startNextDownload();
}

void DownloadManager::cancelDownload()
 {
     httpRequestAborted = true;
     currentDownload->abort();
     emit finished();
 }

void DownloadManager::downloadReadyRead()
{
	if (httpRequestAborted)
		return;

	buffer->write(currentDownload->readAll());
}
