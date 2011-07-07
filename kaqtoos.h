#ifndef KAQTOOS_H
#define KAQTOOS_H

#include <QMainWindow>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QUrl>

#include "downloadmanager.h"

class QMenu;
class QAction;
class QSplitter;
class QTreeView;
class QTabWidget;
class QGraphicsView;

class KQOAuthManager;
class KQOAuthRequest;

class QBuffer;

static const QString String_Version = "0.0.1";

class Kaqtoos : public QMainWindow {
	Q_OBJECT

public:
	Kaqtoos();
	~Kaqtoos();

private slots:
	// OAuth
	void getAccess();
	void onTemporaryTokenReceived(const QString &temporaryToken, const QString &temporaryTokenSecret);
	void onAuthorizationReceived(const QString &token, const QString &verifier);
	void onAccessTokenReceived(const QString &token, const QString &tokenSecret);
	void onAuthorizedRequestDone();
	void onRequestReady(const QByteArray &response);
	void deleteUserOAuthConnection();

	// Network
	void receiveBuffer(QBuffer *buffer, const QString &url);


	// UI
	void openAbout();
	void setupUi();

private:

	// UI
	QTreeView *cattree;
	QTreeView *friendstree;
	QTreeView *groupstree;

	QTabWidget *tableft;
	QTabWidget *tabright;

	QSplitter *splitter;

	QGraphicsView *allProductsView;
	QGraphicsView *popularProductsView;
	QGraphicsView *visitedProductsView;

	QMenu *fileMenu;
	QMenu *helpMenu;

	QAction *connectAction;
	QAction *disconnectAction;
	QAction *aboutAction;
	QAction *exitAction;

	// OAUTH
	static const QString consumerKey;
	static const QString secretKey;

	KQOAuthManager *oauthManager;
	KQOAuthRequest *oauthRequest;
	QSettings oauthSettings;

	bool isOAuthUserConnected;

	// Network
	DownloadManager downloadManager;

	// Urls
	QStringList publicXmlUrls;

	void setupConnections();
	void setupActions();
};

#endif // KAQTOOS_H
