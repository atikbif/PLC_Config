#ifndef PINGRAPHICSRECTITEM_H
#define PINGRAPHICSRECTITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class PinGraphicsRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    int pinNumber;
    bool textBeforeFlag;
    QGraphicsTextItem *textItem;
public:
    PinGraphicsRectItem(QGraphicsItem * parent = 0, QObject *obParent = 0);
    PinGraphicsRectItem(const QRectF & rect, QGraphicsItem * parent = 0, QObject *obParent = 0);
    PinGraphicsRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0, QObject *obParent = 0);
    void setPinNumber(int value) {pinNumber = value;}
    int getPinNumber() const {return pinNumber;}
    bool isTextBefore() const {return textBeforeFlag;}
    void setTextBeforeFlag(bool value) {textBeforeFlag = value;}
    void setTextItem(QGraphicsTextItem *item) {textItem = item;}
    QGraphicsTextItem* getTextItem() {return textItem;}
signals:
    void leftClick(PinGraphicsRectItem *item);
    void rightClick(PinGraphicsRectItem *item);

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PINGRAPHICSRECTITEM_H
