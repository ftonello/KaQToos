#ifndef KAQTOOS_H
#define KAQTOOS_H

#include <QMainWindow>
#include <QSettings>
#include <QString>
#include <QByteArray>

class QMenu;
class QAction;
class QSplitter;
class QTreeView;
class QTabWidget;
class QGraphicsView;
class QLineEdit;
class QPushButton;

class KQOAuthManager;
class KQOAuthRequest;

static const QString String_Version = "0.0.1";

static const QString Consumer_Key = "2b772265f87acdba2a2a469f1138d8a204e0fe889";
static const QString Secret_Key = "7bc36c61752dce13597143e592c1d1f3";

class Kaqtoos : public QMainWindow {
	Q_OBJECT

public:
	Kaqtoos();
	~Kaqtoos();

private slots:
	// get oauth access
	void getAccess();
	void onTemporaryTokenReceived(const QString &temporaryToken, const QString &temporaryTokenSecret);
	void onAuthorizationReceived(const QString &token, const QString &verifier);
	void onAccessTokenReceived(const QString &token, const QString &tokenSecret);
	void onAuthorizedRequestDone();
	void onRequestReady(const QByteArray &response);

	void deleteConnection();

	void updatePublicContent();
	void updatePrivateContent();

	// UI
	void openAbout();
	void setupUi();

private:

	// UI_DEFAULT
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
	KQOAuthManager *oauthManager;
	KQOAuthRequest *oauthRequest;
	QSettings oauthSettings;

	bool isConnected;

	void setupConnections();
	void setupActions();
};

#endif // KAQTOOS_H
