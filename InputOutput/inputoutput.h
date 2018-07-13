#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <QString>

#include "iobase.h"

class InputOutput: public IOBase
{
public:
    static const QString defaultIOState;    // текст, соответствующий состоянию когда вход/выход не используется
private:
    QString groupName;      // разбитие входов/выходов на группы по функционалу
    int code;   //  числовой код назаначения
public:
    InputOutput();
    InputOutput(const IOBase &io);
    QString getGroupName() const {return groupName;}
    void setGroupName(const QString &value) {groupName = value;}
    int getCode() const {return code;}
    void setCode(int value) {code = value;}
};

#endif // INPUTOUTPUT_H
