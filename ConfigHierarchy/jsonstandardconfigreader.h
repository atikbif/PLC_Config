#ifndef JSONSTANDARDCONFIGREADER_H
#define JSONSTANDARDCONFIGREADER_H

#include "standardconfigreader.h"
#include <QJsonObject>

// JSON реализация чтения конфигурации из файла

class JSONStandardConfigReader:public StandardConfigReader
{
    StandardConfigStorage read(const QJsonObject &json);
    void readConfig(StandardConfigStorage &storage, StandardConfigStorage::Device &device,  const QJsonValue &conf);
    void readDevice(StandardConfigStorage &storage, StandardConfigStorage::Type &devType, const QJsonValue &dev);
    void readType(StandardConfigStorage &storage, const QJsonValue &type);
    // StandardConfigReader interface
public:
    StandardConfigStorage readStandardConfig(const QString &fName);
};

#endif // JSONSTANDARDCONFIGREADER_H
