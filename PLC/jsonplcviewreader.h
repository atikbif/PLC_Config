#ifndef JSONPLCVIEWREADER_H
#define JSONPLCVIEWREADER_H

#include <QJsonObject>
#include <map>

#include "iplcviewreader.h"
#include "InputOutput/iobase.h"

class JSONPLCViewReader : public IPLCViewReader
{   
    QVector<PLCView> read(const QJsonObject &json);
    static const std::map<QString,IOBase::IOTYPES> stringTypes;
    void addIOView(PLCView &plcView, const QJsonValue &io);

    // IPLCViewReader interface
public:
    QVector<PLCView> readData(const QString &fName);
};

#endif // JSONPLCVIEWREADER_H
