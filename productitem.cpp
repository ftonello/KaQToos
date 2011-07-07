#include <QBuffer>
#include <QRectF>
#include <QPointF>
#include <QPainter>

#include <QDebug>

#include "productitem.h"

ProductItem::ProductItem(QGraphicsItem *parent)
	: QGraphicsItem(parent),
	  m_imgSize(150, 150),
	  m_size(200, 400)
{
	// Network
	connect(&downloadManager, SIGNAL(getDownload(QBuffer *, const QString &)),
	        this, SLOT(receiveBuffer(QBuffer *, const QString &)));	
}

ProductItem::~ProductItem()
{
	delete m_buffer;
}

QRectF ProductItem::boundingRect() const
{
	// NOTE: just for test
	return QRectF(QPointF(-100, -50), m_size);
}

void ProductItem::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
	painter->drawImage(QPoint(0, 0), m_image);
}

void ProductItem::setId(int id)
{
	m_id = id;
}

void ProductItem::setName(const QString &name)
{
	m_name = name;
}

void ProductItem::setDescription(const QString &desc)
{
	m_desc = desc;
}

void ProductItem::setUrl(const QString &url)
{
	m_url = url;
}

void ProductItem::setImage(const QString &image_url)
{
	downloadManager.append(image_url);
}

void ProductItem::setTime(int time)
{
	m_time = time;
}

void ProductItem::setActualPrice(qreal price)
{
	m_actualPrice = price;
}

void ProductItem::setPrice(qreal price)
{
	m_price = price;
}

// Network slots

void ProductItem::receiveBuffer(QBuffer *buffer, const QString &url)
{
	qDebug() << buffer->readAll();
	qDebug() << url;

	m_buffer = buffer;

	bufferToImage();
}

void ProductItem::bufferToImage()
{
	if (m_image.loadFromData(m_buffer->data())) {
		m_image = m_image.scaled(m_imgSize);
	} else {
		m_image = QImage(m_imgSize, QImage::Format_RGB32);
		m_image.fill(0xFFFFFF); // em branco
	}
}
