#include "ioclassification.h"
#include "inputoutput.h"

IOClassification::IOClassification()
{
    for (size_t i=0;i<InputOutput::IOTYPES::TYPES_COUNT;i++)
        purposeStorage.append(PurposeStorage());
    commonPurposes.append(std::make_pair(InputOutput::defaultIOState,0));
}

QStringList IOClassification::getGroupNamesByType(InputOutput::IOTYPES type)
{
    QStringList result;
    if(type < InputOutput::IOTYPES::TYPES_COUNT) {
        result =  purposeStorage.at(type).getGroupNames();
    }
    return result;
}

void IOClassification::addGroup(InputOutput::IOTYPES type, QString groupName)
{
    if(type >= InputOutput::IOTYPES::TYPES_COUNT) return;
    auto &purpose_map = purposeStorage[type].purposes;
    auto it = purpose_map.find(groupName);
    if(it!=purpose_map.end()) return;
    purpose_map[groupName] = QVector<Purpose>();
}

void IOClassification::addIOPurposeToGroup(InputOutput::IOTYPES type, QString groupName, QString purpose, int code)
{
    if(type >= InputOutput::IOTYPES::TYPES_COUNT) return;
    auto &purpose_map = purposeStorage[type].purposes;
    auto it = purpose_map.find(groupName);
    if(it!=purpose_map.end()) {
        auto ioPurpose = std::make_pair(purpose,code);
        purpose_map[groupName].append(ioPurpose);
    }
    switch(type) {
        case InputOutput::IOTYPES::DISCRETE_INPUT:
            diCodes.insert(code);break;
        case InputOutput::IOTYPES::DISCRETE_OUTPUT:
            doCodes.insert(code);break;
        case InputOutput::IOTYPES::ANALOG_INPUT:
            aiCodes.insert(code);break;
        case InputOutput::IOTYPES::ANALOG_OUTPUT:
            aoCodes.insert(code);break;
        case InputOutput::IOTYPES::DISCRETE_INPUT_OUTPUT:
            break;
        case InputOutput::IOTYPES::TYPES_COUNT:
            break;
    }
}

void IOClassification::addCommonIOPurpose(QString purpose,int code)
{
    commonPurposes.append(std::make_pair(purpose,code));
}

QVector<IOClassification::Purpose> IOClassification::getCommonIOPurposes()
{
    return commonPurposes;
}

QVector<IOClassification::Purpose> IOClassification::getIOPurposesByGroup(InputOutput::IOTYPES type, QString groupName)
{
    if(type >= InputOutput::IOTYPES::TYPES_COUNT) return QVector<Purpose>();
    const auto &purpose_map = purposeStorage[static_cast<size_t>(type)].purposes;
    auto it = purpose_map.find(groupName);
    if(it!=purpose_map.end()) return purpose_map.at(groupName);
    return QVector<Purpose>();
}

IOClassification::AI_TYPE IOClassification::getAIType(int code) const
{
    if(!isAI(code)) return AI_TYPE::UNKNOWN;
    for(auto &pair:aiTypes) {
        auto &ai_set = pair.second;
        if(ai_set.find(code)!=ai_set.end()) return pair.first;
    }
    return AI_TYPE::UNKNOWN;
}

std::pair<QString, QString> IOClassification::getGroupAndPurposeByCode(int code)
{
    for(const auto& v:commonPurposes) {
        if(v.second==code) {return std::make_pair("",v.first);}
    }
    for(auto &storage:purposeStorage) {
        auto &purposes = storage.purposes;
        QStringList grNames = storage.getGroupNames();
        for(QString &gr:grNames) {
            if(purposes.find(gr)!=purposes.end()) {
                auto &purpVector = purposes[gr];
                for(auto &p:purpVector) {
                    if(p.second == code) {return std::make_pair(gr,p.first);}
                }
            }
        }
    }
    return std::make_pair("",InputOutput::defaultIOState);
}

void IOClassification::addAIType(IOClassification::AI_TYPE type, std::set<int> data)
{
    aiTypes[type] = data;
}

QStringList IOClassification::PurposeStorage::getGroupNames() const
{
    QStringList result;
    for(const auto& purpose:purposes) {
        result.append(purpose.first);
    }
    return result;
}
