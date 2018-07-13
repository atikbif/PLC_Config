#ifndef PLCGUI_H
#define PLCGUI_H

#include <QGraphicsView>
#include "PLC/plc.h"
#include "InputOutput/ioclassification.h"
#include "PLCGUI/pingraphicsrectitem.h"
#include <QtSvg/QGraphicsSvgItem>
#include "ConfigHierarchy/standardconfigstorage.h"

class PLCGUI:public QObject
{
    Q_OBJECT
public:
    static const int width = 900;
    static const int height =900;
private:
    QGraphicsView *view;
    PLC &plc;
    IOClassification &ioClassification;
    QGraphicsSvgItem *pSvgItem; // изображение контроллера
    QGraphicsScene *scene;
    std::map<int,PinGraphicsRectItem*> rectItems;   // кликабельные области входов/выходов (ключ - номер контакта контроллера)
    double resetScale = 1.0;    // масштаб
private slots:
    // щелчок мышкой по кликабельной области входа/выхода
    void rectLeftClicked(PinGraphicsRectItem *item);
    void rectRightClicked(PinGraphicsRectItem *item);
public:
    PLCGUI(QGraphicsView *view,PLC &plc, IOClassification &ioClassification,QObject *parent=nullptr);
    void updatePLCGUI();    // перерисовать контроллер в соответствии с текущим состоянием
    StandardConfigStorage::Config newPLC(const QString &fName); // загрузить контроллер из файла
    void zoom_in();
    void zoom_out();
};

#endif // PLCGUI_H
