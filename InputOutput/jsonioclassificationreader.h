#ifndef JSONIOCLASSIFICATIONREADER_H
#define JSONIOCLASSIFICATIONREADER_H

#include <QJsonObject>

#include "iioclassificationreader.h"

class JSONIOClassificationReader:public IIOClassificationReader
{   
    static const std::map<QString,IOBase::IOTYPES> stringTypes;
    static const std::map<QString,IOClassification::AI_TYPE> aiTypes;
    void read(const QJsonObject &json, IOClassification &ioClassification);
    void createDIOPurposesFromDiAndDO(IOClassification &ioClassification);
    // IIOClassificationReader interface
public:
    void readIOClassification(const QString &fName, IOClassification &ioClassification);
};

#endif // JSONIOCLASSIFICATIONREADER_H
