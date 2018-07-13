#ifndef BOILPINFRAMCONVERTER_H
#define BOILPINFRAMCONVERTER_H

#include "pinframconverter.h"
#include <array>


class BoilPinFRAMConverter : public PinFRAMConverter
{
    // номера заводских установок (EEi) для входов/выходов
    const std::array<int,8> ai_addr { {616,618,620,622,624,626,628,630} };
    const std::array<int,2> ao_addr { {808,810} };
    const std::array<int,24> di_addr { {832,834,836,838,840,842,844,846,848,850,852,854,856,858,860,862,864,866,868,870,872,874,876,878} };
    const std::array<int,24> do_addr { {976,978,980,982,984,986,988,990,992,994,996,998,1000,1002,1004,1006,1008,1010,1012,1014,1016,1018,1020,1022} };

    std::map<int,int> getClearFram() const;


    void updateDI(InputOutput &io, std::map<int,int> &framMap, IOClassification &ioClassification, PLC &plc);
    void updateAI(InputOutput &io, std::map<int,int> &framMap, IOClassification &ioClassification, PLC &plc);
    void updateDO(InputOutput &io, std::map<int,int> &framMap, IOClassification &ioClassification, PLC &plc);
    void updateAO(InputOutput &io, std::map<int,int> &framMap, IOClassification &ioClassification, PLC &plc);
    void updateDIO(InputOutput &io, std::map<int,int> &framMap, IOClassification &ioClassification, PLC &plc);
    void updateIO(InputOutput &io, int code, IOClassification &ioClassification, PLC &plc);

    // PinFRAMConverter interface
public:
    static const int startOffset = 0x7B00;  // адрес ячейки FRAM с первой заводской установкой
    QVector<FramCoil> pinToFram(IOClassification &ioClassification, PLC &plc);

    // PinFRAMConverter interface
public:
    QVector<int> getFramAddressList();
    void framToPLC(QVector<FramCoil> framContent, IOClassification &ioClassification, PLC &plc);
};

#endif // BOILPINFRAMCONVERTER_H
