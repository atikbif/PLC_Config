#ifndef IOBASE_H
#define IOBASE_H

#include <map>
#include <QString>

class IOBase
{
public:
    IOBase() = default;
    enum IOTYPES{DISCRETE_INPUT,DISCRETE_OUTPUT,DISCRETE_INPUT_OUTPUT,ANALOG_INPUT,ANALOG_OUTPUT,TYPES_COUNT};
private:
    int pinNumber;  // номер контакта контроллера
    int inputNumber;    // номер входа
    int outputNumber;   // номер выхода
    IOTYPES type;
public:
    int getPinNumber() const {return pinNumber;}
    void setPinNumber(int value) {pinNumber = value;}
    int getInputNumber() const {return inputNumber;}
    void setInputNumber(int value) {inputNumber = value;}
    int getOutputNumber() const {return outputNumber;}
    void setOutputNumber(int value) {outputNumber = value;}
    IOTYPES getType() const {return type;}
    void setType(IOTYPES value) {type = value;}
};

#endif // IOBASE_H
