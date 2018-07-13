#ifndef PINFRAMCONVERTER_H
#define PINFRAMCONVERTER_H

#include <utility>
#include "PLC/plc.h"
#include "InputOutput/ioclassification.h"

// преобразное конфигурации контроллера в таблицу ячеек FRAM памяти и обратно

class PinFRAMConverter
{
public:
    using FramCoil = std::pair<int,int>; // ячейка FRAM памяти (адрес/значение)
    virtual QVector<FramCoil> pinToFram(IOClassification &ioClassification, PLC &plc) = 0;
    virtual QVector<int> getFramAddressList() = 0;
    virtual void framToPLC(QVector<FramCoil> framContent, IOClassification &ioClassification, PLC &plc) = 0;
};

#endif // PINFRAMCONVERTER_H
