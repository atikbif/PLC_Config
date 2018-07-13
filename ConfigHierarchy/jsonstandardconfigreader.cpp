#include "jsonstandardconfigreader.h"
#include "jsondevicepatternreader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

StandardConfigStorage JSONStandardConfigReader::read(const QJsonObject &json)
{
    StandardConfigStorage storage;
    auto types = json["type"].toArray();
    for(const auto &type:types) {
        readType(storage,type);
    }
    return storage;
}

void JSONStandardConfigReader::readConfig(StandardConfigStorage &storage, StandardConfigStorage::Device &device, const QJsonValue &conf)
{
    QString confFile = ":/JSON/" + conf.toString();
    JSONDevicePatternReader confReader;
    auto confInfo = confReader.getConfigInfo(confFile);
    QString confName = confInfo.name;
    QString confComment = confInfo.comment;
    QString confPLC =confInfo.plc;
    auto deviceConfig = storage.createConfig(confName,confFile,confComment,confPLC);
    storage.addConfigToDevice(deviceConfig,device);
}

void JSONStandardConfigReader::readDevice(StandardConfigStorage &storage, StandardConfigStorage::Type &devType, const QJsonValue &dev)
{
    auto dOb = dev.toObject();
    QString devName = dOb["name"].toString();
    auto device = storage.createDevice(devName);
    auto configs = dOb["config_list"].toArray();
    for(const auto &conf:configs) {
        readConfig(storage, device, conf);
    }
    storage.addDeviceToType(device, devType);
}

void JSONStandardConfigReader::readType(StandardConfigStorage &storage, const QJsonValue &type)
{
    auto tOb = type.toObject();
    int num = tOb["num"].toInt();
    QString tName = tOb["name"].toString();
    auto deviceType = storage.createType(tName,num);
    auto devices = tOb["device_list"].toArray();
    for(const auto &dev:devices) {
        readDevice(storage,deviceType,dev);
    }
    storage.addTypeToConfig(deviceType);
}

StandardConfigStorage JSONStandardConfigReader::readStandardConfig(const QString &fName)
{
    QFile loadFile(fName);

    if (loadFile.open(QIODevice::ReadOnly)) {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        return read(loadDoc.object());
    }else {
        QString message = "Couldn't open file " + fName;
        qWarning(message.toStdString().c_str());
    }
    return StandardConfigStorage();
}
