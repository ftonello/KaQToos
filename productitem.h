#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#include <QGraphicsItem>
#include <QString>
#include <QUrl>
#include <QImage>

#include "downloadmanager.h"

class QBuffer;

class ProductItem : public QObject, public QGraphicsItem {
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	ProductItem(QGraphicsItem *parent = 0);
	~ProductItem();

	virtual QRectF boundingRect() const;

	virtual void paint(QPainter *painter,
	                   const QStyleOptionGraphicsItem *option,
	                   QWidget *widget);

	void setId(int id);
	void setName(const QString &name);
	void setDescription(const QString &desc);
	void setUrl(const QString &url);
	void setImage(const QString &image_url);
	void setTime(int time);
	void setActualPrice(qreal price);
	void setPrice(qreal price);

private slots:
	// Network
	void receiveBuffer(QBuffer *buffer, const QString &url);

private:
	// network
	DownloadManager downloadManager;

	// properties
	int      m_id;
	QString  m_name;
	QString  m_desc;
	QUrl     m_url;
	QImage   m_image;
	int      m_time;
	qreal    m_actualPrice;
	qreal    m_price;

	QBuffer *m_buffer;

	// size default
	QSize    m_imgSize; //image default
	QSize    m_size;    //all drawing size

	void bufferToImage(); // called by the receiveBuffer
};

#endif // PRODUCTITEM_H
