#include "plcview.h"
#include "InputOutput/inputoutput.h"


IOView PLCView::getIOViewByPinNumber(int num) const
{
    for(const auto &io:ioViews) {
        if(io.getPinNumber()==num) return io;
    }
    return IOView();
}

PLC PLCView::getEmptyPLC() const
{
    PLC plc;
    for(const auto &io:ioViews) {
        InputOutput plcIO = static_cast<IOBase>(io);
        plc.addInputOutput(plcIO);
    }
    return plc;
}
