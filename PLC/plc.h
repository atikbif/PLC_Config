#ifndef PLC_H
#define PLC_H

#include <QString>
#include <QVector>
#include <memory>

#include "InputOutput/inputoutput.h"
#include "InputOutput/ioclassification.h"

class PLC
{
private:
    QString name;
    QVector<InputOutput> io_list;
public:
    PLC() = default;
    explicit PLC(const QString &name);
    QString getName() const {return name;}
    void addInputOutput(const InputOutput &io) {io_list.append(io);}
    QVector<InputOutput::IOTYPES> getInputOutputTypes() const;
    QVector<InputOutput> getInputOutputListByType(InputOutput::IOTYPES type);
    InputOutput getInputOutputByPinNum(int num, bool *res=nullptr);
    void updateInputOutput(const InputOutput &value);
    // задать функции входов/выходов (pins.first - номер контакта, pins.second - код функции)
    void setIOState(std::map<int,int> pins, IOClassification &ioClassification);
};

#endif // PLC_H
