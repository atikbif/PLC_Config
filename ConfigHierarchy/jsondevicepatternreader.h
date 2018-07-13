#ifndef JSONDEVICEPATTERNREADER_H
#define JSONDEVICEPATTERNREADER_H

#include "devicepatternreader.h"
#include <QJsonObject>

class JSONDevicePatternReader : public DevicePatternReader
{ 
    std::map<int, int> read(const QJsonObject &json);
    // DevicePatternReader interface
public:
    std::map<int, int> getDevicePattern(const QString &pName);
    StandardConfigStorage::Config getConfigInfo(const QString &pName);
};

#endif // JSONDEVICEPATTERNREADER_H
