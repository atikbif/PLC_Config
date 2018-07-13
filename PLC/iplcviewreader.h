#ifndef IPLCVIEWREADER_H
#define IPLCVIEWREADER_H

#include "plcview.h"

class IPLCViewReader
{
public:
    virtual QVector<PLCView> readData(const QString &fName)=0;
};

#endif // IPLCVIEWREADER_H
