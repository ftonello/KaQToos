#include <QGraphicsView>
#include <QGraphicsScene>

#include <QTreeView>
#include <QTabWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QGroupBox>
#include <QProgressDialog>

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>
#include <QSize>
#include <QUrl>
#include <QBuffer>

#include <QDebug>

#include <QtKOAuth>

#include "kaqtoos.h"
#include "productitem.h"

// static variables
const QString Kaqtoos::consumerKey = "2b772265f87acdba2a2a469f1138d8a204e0fe889";
const QString Kaqtoos::secretKey = "7bc36c61752dce13597143e592c1d1f3";

Kaqtoos::Kaqtoos()
	: QMainWindow(),
	  downloadManager(this),
	  isOAuthUserConnected(false)
{
	oauthRequest = new KQOAuthRequest();
	oauthManager = new KQOAuthManager(this);

	setupUi();
	setupActions();
	setupConnections();
	
	publicXmlUrls <<
		QString("http://api.kactoos.com/br/api/products/get-product-list/format/xml/oauth_consumer_key/%1/limit/30/orderby/new-products").arg(consumerKey) <<
		QString("http://api.kactoos.com/br/api/products/get-product-list/format/xml/oauth_consumer_key/%1/limit/30/orderby/popular").arg(consumerKey) <<
		QString("http://api.kactoos.com/br/api/products/get-product-list/format/xml/oauth_consumer_key/%1/limit/30/orderby/economic").arg(consumerKey);

	// start downloading
	downloadManager.append(publicXmlUrls);

	// NOTE: just for test
	QGraphicsScene *scene = new QGraphicsScene();

	ProductItem *item = new ProductItem();
	item->setImage("http://www.kactoos.com/libraries/thumb/?src=/images/products/5791_3556942391.jpg");
	
	scene->addItem(item);
	allProductsView->setScene(scene);

	setWindowTitle(tr("KaQToos"));
	resize(QSize(600, 480));
}

Kaqtoos::~Kaqtoos()
{
	delete oauthRequest;
	delete oauthManager;
}

// OAuth slots

void Kaqtoos::getAccess()
{
	oauthRequest->initRequest(KQOAuthRequest::TemporaryCredentials, QUrl("http://api.kactoos.com/api/oauth/request_token"));
	oauthRequest->setConsumerKey(consumerKey);
	oauthRequest->setConsumerSecretKey(secretKey);

	oauthManager->setHandleUserAuthorization(true);

	oauthManager->executeRequest(oauthRequest);
}

void Kaqtoos::onTemporaryTokenReceived(const QString &token, const QString &tokenSecret)
{
	qDebug() << "Temporary token received: " << token << tokenSecret;

	QUrl userAuthURL("http://api.kactoos.com/api/oauth/authorize");

	if(oauthManager->lastError() == KQOAuthManager::NoError) {
		qDebug() << "Asking for user's permission to access protected resources. Opening URL: " << userAuthURL;
		oauthManager->getUserAuthorization(userAuthURL);
	}

}

void Kaqtoos::onAuthorizationReceived(const QString &token, const QString &verifier)
{
	qDebug() << "User authorization received: " << token << verifier;

	oauthManager->getUserAccessTokens(QUrl("http://api.kactoos.com/api/oauth/access_token"));
	if(oauthManager->lastError() != KQOAuthManager::NoError) {
		qDebug () << "Error";
	}
}

void Kaqtoos::onAccessTokenReceived(const QString &token, const QString &tokenSecret)
{
	qDebug() << "Access token received: " << token << tokenSecret;

	isOAuthUserConnected = false;
	connectAction->setEnabled(false);
	disconnectAction->setEnabled(true);

	oauthSettings.setValue("kaqtoos_oauth_token", token);
	oauthSettings.setValue("kaqtoos_oauth_token_secret", tokenSecret);

	// TODO: fazer os requests privados (friends, groups, etc)

	qDebug() << "Access tokens now stored.";
}

void Kaqtoos::onAuthorizedRequestDone()
{
	qDebug() << "Request sent to Kactoos!";
}

void Kaqtoos::onRequestReady(const QByteArray &response)
{
	qDebug() << "Response from the service: " << response;
}

void Kaqtoos::deleteUserOAuthConnection()
{
	connectAction->setEnabled(true);
	disconnectAction->setEnabled(false);

	oauthSettings.setValue("kaqtoos_oauth_token", "");
	oauthSettings.setValue("kaqtoos_oauth_token_secret", "");

	// TODO: limpar todos os request privados (friends, groups, etc)
}

// Network slots

void Kaqtoos::receiveBuffer(QBuffer *buffer, const QString &url)
{
	qDebug() << buffer->readAll();
	qDebug() << url;

	delete buffer;
}

// UI slots

void Kaqtoos::openAbout()
{
	QMessageBox::about(this, tr("Sobre KaQToos"),
	                   tr("<h2>KaQToos %1</h2>"
	                      "<p>Copyright &copy; 2011 Felipe Ferreri Tonello</p>"
	                      "<p>...</p>").arg(String_Version));
}

void Kaqtoos::setupUi()
{
	fileMenu = menuBar()->addMenu(tr("&Arquivo"));
	helpMenu = menuBar()->addMenu(tr("&Ajuda"));

	tableft = new QTabWidget();
	tabright = new QTabWidget();
	
	cattree = new QTreeView();
	friendstree = new QTreeView();
	groupstree = new QTreeView();

	allProductsView = new QGraphicsView();
	popularProductsView = new QGraphicsView();
	visitedProductsView = new QGraphicsView();

	tableft->setMovable(true);
	tableft->addTab(cattree, tr("Categorias"));
	tableft->addTab(friendstree, tr("Amigos"));
	tableft->addTab(groupstree, tr("Grupos"));

	tabright->setMovable(true);
	tabright->addTab(allProductsView, tr("Novos"));
	tabright->addTab(popularProductsView, tr("Populares"));
	tabright->addTab(visitedProductsView, tr("Economicos"));

	splitter = new QSplitter(Qt::Horizontal);
	splitter->addWidget(tableft);
	splitter->addWidget(tabright);
	splitter->setStretchFactor(1, 1);

	setCentralWidget(splitter);
}

void Kaqtoos::setupConnections()
{
	// OAUTH
	connect(oauthManager, SIGNAL(temporaryTokenReceived(QString,QString)),
	        this, SLOT(onTemporaryTokenReceived(QString, QString)));

	connect(oauthManager, SIGNAL(authorizationReceived(QString,QString)),
	        this, SLOT(onAuthorizationReceived(QString, QString)));

	connect(oauthManager, SIGNAL(accessTokenReceived(QString,QString)),
	        this, SLOT(onAccessTokenReceived(QString,QString)));

	connect(oauthManager, SIGNAL(requestReady(QByteArray)),
	        this, SLOT(onRequestReady(QByteArray)));

	// Network
	connect(&downloadManager, SIGNAL(getDownload(QBuffer *, const QString &)),
	        this, SLOT(receiveBuffer(QBuffer *, const QString &)));

	// UI
	connect(connectAction, SIGNAL(triggered()), this, SLOT(getAccess()));
	connect(disconnectAction, SIGNAL(triggered()), this, SLOT(deleteUserOAuthConnection()));

	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(openAbout()));
}

void Kaqtoos::setupActions()
{
	connectAction = new QAction(tr("Conectar-se ..."), this);
	fileMenu->addAction(connectAction);

	disconnectAction = new QAction(tr("Desconectar-se ..."), this);
	disconnectAction->setEnabled(false);
	fileMenu->addAction(disconnectAction);
	fileMenu->addSeparator();

	exitAction = new QAction(tr("Sair"), this);
	exitAction->setMenuRole(QAction::QuitRole);
	exitAction->setShortcut(QKeySequence::Quit);
	fileMenu->addAction(exitAction);

	aboutAction = new QAction(tr("Sobre Kaqtoos"), this);
	aboutAction->setMenuRole(QAction::AboutRole);
	helpMenu->addAction(aboutAction);
}
