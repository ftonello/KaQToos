#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#include <QGraphicsItem>
#include <QString>
#include <QUrl>
#include <QImage>

class ProductItem : public QGraphicsItem {
public:
	ProductItem(QGraphicsItem *parent = 0);

	void paint(QPainter *painter,
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

private:
	int      m_id;
	QString  m_name;
	QString  m_desc;
	QUrl     m_url;
	QImage   m_image;
	int      m_time;
	qreal    m_actualPrice;
	qreal    m_price;

	// size default
	QSize    m_size;
};

#endif // PRODUCTITEM_H
