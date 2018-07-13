#include <algorithm>

#include "boilpinframconverter.h"
#include "InputOutput/aiconfig.h"


std::map<int, int> BoilPinFRAMConverter::getClearFram() const
{
    std::map<int,int> map;

    for(auto addr:ai_addr) {
        map[startOffset+addr] = 0;
        map[startOffset+1+addr] = 0;
    }
    for(auto addr:ao_addr) {
        map[startOffset+addr] = 0;
        map[startOffset+1+addr] = 0;
    }
    for(auto addr:di_addr) {
        map[startOffset+addr] = 0;
        map[startOffset+1+addr] = 0;
    }
    for(auto addr:do_addr) {
        map[startOffset+addr] = 0;
        map[startOffset+1+addr] = 0;
    }

    return map;
}

void BoilPinFRAMConverter::updateDI(InputOutput &io, std::map<int,int> &framMap, IOClassification &ioClassification, PLC &plc)
{
    std::size_t inpNum = io.getInputNumber();
    if(di_addr.size()>=inpNum) {
        int addr = di_addr[inpNum-1];
        int code = framMap[addr] + (framMap[addr+1]<<8);
        updateIO(io,code,ioClassification,plc);
    }
}

void BoilPinFRAMConverter::updateAI(InputOutput &io, std::map<int, int> &framMap, IOClassification &ioClassification, PLC &plc)
{
    std::size_t inpNum = io.getInputNumber();
    if(ai_addr.size()>=inpNum) {
        int addr = ai_addr[inpNum-1];
        int code = framMap[addr] + (framMap[addr+1]<<8);
        updateIO(io,code,ioClassification,plc);
    }
}

void BoilPinFRAMConverter::updateDO(InputOutput &io, std::map<int, int> &framMap, IOClassification &ioClassification, PLC &plc)
{
    std::size_t outNum = io.getOutputNumber();
    if(do_addr.size()>=outNum) {
        int addr = do_addr[outNum-1];
        int code = framMap[addr] + (framMap[addr+1]<<8);
        updateIO(io,code,ioClassification,plc);
    }
}

void BoilPinFRAMConverter::updateAO(InputOutput &io, std::map<int, int> &framMap, IOClassification &ioClassification, PLC &plc)
{
    std::size_t outNum = io.getOutputNumber();
    if(ao_addr.size()>=outNum) {
        int addr = ao_addr[outNum-1];
        int code = framMap[addr] + (framMap[addr+1]<<8);
        updateIO(io,code,ioClassification,plc);
    }
}

void BoilPinFRAMConverter::updateDIO(InputOutput &io, std::map<int, int> &framMap, IOClassification &ioClassification, PLC &plc)
{
    std::size_t inpNum = io.getInputNumber();
    std::size_t outNum = io.getOutputNumber();
    int diCode = 0;
    int doCode = 0;
    if(di_addr.size()>=inpNum) {
        int addr = di_addr[inpNum-1];
        diCode = framMap[addr] + (framMap[addr+1]<<8);
    }
    if(do_addr.size()>=outNum) {
        int addr = do_addr[outNum-1];
        doCode = framMap[addr] + (framMap[addr+1]<<8);
    }
    if(diCode) {
        updateIO(io,diCode,ioClassification,plc);
    }else if(doCode) {
        updateIO(io,doCode,ioClassification,plc);
    }
}

void BoilPinFRAMConverter::updateIO(InputOutput &io, int code, IOClassification &ioClassification, PLC &plc)
{
    auto groupAndPurpose = ioClassification.getGroupAndPurposeByCode(code);
    io.setCode(code);
    io.setGroupName(groupAndPurpose.first);
    plc.updateInputOutput(io);
}

QVector<PinFRAMConverter::FramCoil> BoilPinFRAMConverter::pinToFram(IOClassification &ioClassification, PLC &plc)
{
    QVector<FramCoil> data;

    std::map<int,int> map = getClearFram();     // адрес ячейки/ содержимое ячейки

    const auto& ioTypes = plc.getInputOutputTypes();
    for(const auto &ioType:ioTypes) {
        const auto &ioList = plc.getInputOutputListByType(ioType);
        for(auto& io:ioList) {
            int code = io.getCode();
            std::size_t inpNum = io.getInputNumber();
            std::size_t outNum = io.getOutputNumber();

            if(ioClassification.isAI(code)) {
                if(ai_addr.size()>=inpNum) {
                    map[startOffset+ai_addr.at(inpNum-1)] = code&0xFF;
                    map[startOffset+1+ai_addr.at(inpNum-1)] = code>>8;
                }
            }else if(ioClassification.isAO(code)) {
                if(ao_addr.size()>=outNum) {
                    map[startOffset+ao_addr.at(outNum-1)] = code&0xFF;
                    map[startOffset+1+ao_addr.at(outNum-1)] = code>>8;
                }
            }else if(ioClassification.isDI(code)) {
                if(di_addr.size()>=inpNum) {
                    map[startOffset+di_addr.at(inpNum-1)] = code&0xFF;
                    map[startOffset+1+di_addr.at(inpNum-1)] = code>>8;
                }
            }else if(ioClassification.isDO(code)) {
                if(do_addr.size()>=outNum) {
                    map[startOffset+do_addr.at(outNum-1)] = code&0xFF;
                    map[startOffset+1+do_addr.at(outNum-1)] = code>>8;
                }
            }

        }
    }

    for(auto & pair:AIConfig::eeBufMap) {
        data.append(std::make_pair(startOffset+pair.first,pair.second&0xFF));
        data.append(std::make_pair(startOffset+pair.first+1,pair.second>>8));
    }

    for(const auto &pair:map) {
        data.append(std::make_pair(pair.first,pair.second));
    }
    //sort
    //std::sort(data.begin(),data.end());//,[](std::pair<int,int> pair1,std::pair<int,int> pair2){return pair1.first<pair2.first;});
    return data;
}

QVector<int> BoilPinFRAMConverter::getFramAddressList()
{
    QVector<int> addrList;
    for(auto addr:ai_addr) {
        addrList.append(startOffset+addr);
        addrList.append(startOffset+1+addr);
    }
    for(auto addr:ao_addr) {
        addrList.append(startOffset+addr);
        addrList.append(startOffset+1+addr);
    }
    for(auto addr:di_addr) {
        addrList.append(startOffset+addr);
        addrList.append(startOffset+1+addr);
    }
    for(auto addr:do_addr) {
        addrList.append(startOffset+addr);
        addrList.append(startOffset+1+addr);
    }
    return addrList;
}

void BoilPinFRAMConverter::framToPLC(QVector<FramCoil> framContent, IOClassification &ioClassification, PLC &plc)
{
    std::map<int,int> framMap;
    for(const auto &coil:framContent) {
        framMap[coil.first-startOffset] = coil.second;
    }

    const auto& ioTypes = plc.getInputOutputTypes();
    for(const auto &ioType:ioTypes) {
        const auto &ioList = plc.getInputOutputListByType(ioType);
        for(auto io:ioList) {
            switch(ioType) {
                case IOBase::IOTYPES::DISCRETE_INPUT:
                    updateDI(io,framMap,ioClassification,plc);
                    break;
                case IOBase::IOTYPES::DISCRETE_OUTPUT:
                    updateDO(io,framMap,ioClassification,plc);
                    break;
                case IOBase::IOTYPES::ANALOG_INPUT:
                    updateAI(io,framMap,ioClassification,plc);
                    break;
                case IOBase::IOTYPES::ANALOG_OUTPUT:
                    updateAO(io,framMap,ioClassification,plc);
                    break;
                case IOBase::IOTYPES::DISCRETE_INPUT_OUTPUT:
                    updateDIO(io,framMap,ioClassification,plc);
                case IOBase::IOTYPES::TYPES_COUNT:
                    break;
            }
        }
    }
    for(auto & pair:AIConfig::eeBufMap) {
        auto it1 = framMap.find(pair.first);
        auto it2 = framMap.find(pair.first+1);
        if(it1!=framMap.end() && it2 != framMap.end()) {
            AIConfig::eeBufMap[pair.first] = framMap[pair.first] + (framMap[pair.first+1]<<8);
        }
    }
}
