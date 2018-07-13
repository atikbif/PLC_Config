#include "plc.h"
#include <set>

PLC::PLC(const QString &name):name(name)
{

}

QVector<InputOutput::IOTYPES> PLC::getInputOutputTypes() const
{
    std::set<InputOutput::IOTYPES> types;
    for(const auto &io:io_list) {
        types.insert(io.getType());
    }
    QVector<InputOutput::IOTYPES> res;
    for(const auto &type:types) res.append(type);
    return res;
}

QVector<InputOutput> PLC::getInputOutputListByType(InputOutput::IOTYPES type)
{
    QVector<InputOutput> res;
    for(const auto &io:io_list) {
        if(io.getType()==type) res.append(io);
    }
    return res;
}

InputOutput PLC::getInputOutputByPinNum(int num, bool *res)
{
    if(res!=nullptr)  *res = false;
    for(auto io:io_list) {
        if(io.getPinNumber()==num) {
            if(res!=nullptr) *res = true;
            return io;
        }
    }
    return InputOutput();
}

void PLC::updateInputOutput(const InputOutput &value)
{
    for(auto &io:io_list) {
        if(io.getPinNumber()==value.getPinNumber()) {
            io = value;
        }
    }
}

void PLC::setIOState(std::map<int, int> pins, IOClassification &ioClassification)
{
    for(const auto pin:pins) {
        int pinNum = pin.first;
        int code = pin.second;
        for(auto &io:io_list) {
            if(io.getPinNumber()==pinNum) {
                auto p = ioClassification.getGroupAndPurposeByCode(code);
                io.setGroupName(p.first);
                io.setCode(code);
            }
        }
    }
}
