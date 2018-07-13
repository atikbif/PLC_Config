#ifndef DEVICEPATTERNREADER_H
#define DEVICEPATTERNREADER_H

#include <map>
#include <QString>
#include "ConfigHierarchy/standardconfigstorage.h"

class DevicePatternReader
{
public:
    virtual std::map<int,int> getDevicePattern(const QString &pName) = 0;
    virtual StandardConfigStorage::Config getConfigInfo(const QString &pName) = 0;
};

#endif // DEVICEPATTERNREADER_H
