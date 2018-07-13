#ifndef PLCVIEW_H
#define PLCVIEW_H

#include <QString>
#include <QVector>

#include "ioview.h"
#include "plc.h"

class PLCView
{
    QString name;   // имя контроллера
    QString imageFileName;  // файл с изображением контроллера
    QVector<IOView> ioViews;
    QString color;  // цвет кликабельной области входов/выходов
    double opacity; // прозрачность кликабельной области
public:
    PLCView() = default;
    QString getName() const {return name;}
    void setName(const QString &value) {name = value;}
    QString getColor() const {return color;}
    void setColor(const QString &value) {color = value;}
    double getOpacity() const {return opacity;}
    void setOpacity(double value) {opacity = value;}
    QString getImageFileName() const {return imageFileName;}
    void setImageFileName(const QString &fName) {imageFileName = fName;}
    const QVector<IOView> getIOViews() const {return ioViews;}
    IOView getIOViewByPinNumber(int num) const;
    void addIOView(const IOView &io) {ioViews.append(io);}
    PLC getEmptyPLC() const;
};

#endif // PLCVIEW_H
