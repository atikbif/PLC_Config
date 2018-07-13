#include "pingraphicsrectitem.h"
#include <QBrush>
#include <QGraphicsSceneMouseEvent>

PinGraphicsRectItem::PinGraphicsRectItem(QGraphicsItem *parent, QObject *obParent):
    QObject(obParent),QGraphicsRectItem(parent)
{
    pinNumber = 0;
    textBeforeFlag = false;
    textItem = nullptr;
}

PinGraphicsRectItem::PinGraphicsRectItem(const QRectF &rect, QGraphicsItem *parent, QObject *obParent):
    QObject(obParent),QGraphicsRectItem(rect,parent)
{
    pinNumber = 0;
    textBeforeFlag = false;
    textItem = nullptr;
}

PinGraphicsRectItem::PinGraphicsRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent, QObject *obParent):
    QObject(obParent),QGraphicsRectItem(x,y,width,height,parent)
{
    pinNumber = 0;
    textBeforeFlag = false;
    textItem = nullptr;
}

void PinGraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) emit leftClick(this);
    else if(event->button()==Qt::RightButton) emit rightClick(this);
    QGraphicsItem::mousePressEvent(event);
}

