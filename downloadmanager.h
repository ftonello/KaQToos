#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>

class QProgressDialog;
class QBuffer;

class DownloadManager: public QObject {
	Q_OBJECT
public:
	DownloadManager(QObject *parent = 0);
	~DownloadManager();

	void append(const QUrl &url);
	void append(const QStringList &urlList);

signals:
	void finished();
	void getDownload(QBuffer *, const QString &);

private slots:
	void startNextDownload();
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void downloadFinished();
	void downloadReadyRead();
	void cancelDownload();

private:
	QNetworkAccessManager manager;
	QQueue<QUrl> downloadQueue;
	QNetworkReply *currentDownload;

	QBuffer *buffer; // client should delete
	QString currentUrl;

	QProgressDialog *progressDialog;

	bool httpRequestAborted;
};

#endif
