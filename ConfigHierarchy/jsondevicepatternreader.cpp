#include "jsondevicepatternreader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>


std::map<int, int> JSONDevicePatternReader::read(const QJsonObject &json)
{
    std::map<int,int> res;
    auto pins = json["pins"].toArray();
    for(const auto& pin:pins) {
        auto pinOb = pin.toObject();
        res.insert(std::make_pair(pinOb["pin"].toInt(),pinOb["code"].toInt()));
    }
    return res;
}

std::map<int, int> JSONDevicePatternReader::getDevicePattern(const QString &pName)
{
    QFile loadFile(pName);

    if (loadFile.open(QIODevice::ReadOnly)) {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        return read(loadDoc.object());
    }else {
        QString message = "Couldn't open file " + pName;
        qWarning(message.toStdString().c_str());
    }
    return std::map<int,int>();
}

StandardConfigStorage::Config JSONDevicePatternReader::getConfigInfo(const QString &pName)
{
    QFile loadFile(pName);
    StandardConfigStorage::Config config;

    if (loadFile.open(QIODevice::ReadOnly)) {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        auto ob = loadDoc.object();
        config.name = ob["config"].toString();
        config.file = pName;
        config.plc = ob["plc"].toString();
        config.comment = ob["comment"].toString();
        config.baseType = ob["type"].toString();
        config.baseDevice = ob["device"].toString();
        config.defaultEE = ob["EEFile"].toString();
    }else {
        QString message = "Couldn't open file " + pName;
        qWarning(message.toStdString().c_str());
    }
    return config;
}
