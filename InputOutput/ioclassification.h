#ifndef IOCLASSIFICATION_H
#define IOCLASSIFICATION_H

#include <QStringList>
#include <map>
#include <QVector>
#include <set>

#include "inputoutput.h"

// Классификация назначений входов/выходов

class IOClassification
{
public:
    using Purpose = std::pair<QString,int>; // текстовое описание назначения, код
    enum class AI_TYPE {PRESSURE,TEMPERATURE,LEVEL,FREQUENCY,UNKNOWN};
private:
    struct PurposeStorage
    {
        QStringList getGroupNames() const;
        std::map<QString,QVector<Purpose>> purposes;
    };
    QVector<PurposeStorage> purposeStorage;
    QVector<Purpose> commonPurposes;
    std::set<int> diCodes;
    std::set<int> doCodes;
    std::set<int> aiCodes;
    std::set<int> aoCodes;
    std::map<AI_TYPE,std::set<int>> aiTypes;
public:
    IOClassification();
    QStringList getGroupNamesByType(InputOutput::IOTYPES type);
    void addGroup(InputOutput::IOTYPES type, QString groupName);
    void addIOPurposeToGroup(InputOutput::IOTYPES type, QString groupName, QString purpose, int code);
    void addCommonIOPurpose(QString purpose, int code);
    QVector<Purpose> getCommonIOPurposes();
    QVector<Purpose> getIOPurposesByGroup(InputOutput::IOTYPES type, QString groupName);
    bool isDI(int code) const {return diCodes.count(code);}
    bool isDO(int code) const {return doCodes.count(code);}
    bool isAI(int code) const {return aiCodes.count(code);}
    bool isAO(int code) const {return aoCodes.count(code);}
    AI_TYPE getAIType(int code) const;
    std::pair<QString,QString> getGroupAndPurposeByCode(int code);
    void addAIType(AI_TYPE type, std::set<int> data);
};

#endif // IOCLASSIFICATION_H
