#include <QHttp>

#include "productitem.h"

ProductItem::ProductItem(QGraphicsItem *parent)
	: QGraphicsItem(parent),
	  m_size(40, 40)
{
	
}

void ProductItem::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
	
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
	QBuffer buf;

	if (buf.open(QBuffer::ReadWrite)) {
		QHttp http;
		if (http.get(image_url, &buf)) {
			m_image = QImage(buf.data().constData(), m_size.width(), m_size.height(), QImage::Format_RGB32);

			http.close();

			return ;
		} 
	}

	// tratamento de erro
	m_image = QImage(m_size, QImage::Format_RGB32);
	m_iamge.fill(0xFFFFFF); // em branco
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
