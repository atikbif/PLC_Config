#include "jsonioclassificationreader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

const std::map<QString,IOBase::IOTYPES> JSONIOClassificationReader::stringTypes = {
    {"di", IOBase::IOTYPES::DISCRETE_INPUT},
    {"do", IOBase::IOTYPES::DISCRETE_OUTPUT},
    {"ai", IOBase::IOTYPES::ANALOG_INPUT},
    {"ao", IOBase::IOTYPES::ANALOG_OUTPUT}
};

const std::map<QString,IOClassification::AI_TYPE> JSONIOClassificationReader::aiTypes = {
    {"Pressure",IOClassification::AI_TYPE::PRESSURE},
    {"Temperature",IOClassification::AI_TYPE::TEMPERATURE},
    {"Level",IOClassification::AI_TYPE::LEVEL},
    {"Frequency",IOClassification::AI_TYPE::FREQUENCY}
};

void JSONIOClassificationReader::read(const QJsonObject &json, IOClassification &ioClassification)
{
    QJsonArray commonPurposes = json["common"].toArray();
    for(const auto &purpose:commonPurposes) {
        auto ob = purpose.toObject();
        ioClassification.addCommonIOPurpose(ob["data"].toString(),ob["code"].toInt());
    }
    for(const auto &type:stringTypes) {
        QString strType = type.first;
        IOBase::IOTYPES ioType = type.second;
        QJsonArray groups = json[strType].toArray();
        for(const auto &groupValue:groups) {
            QJsonObject group = groupValue.toObject();
            QString groupName = group["groupName"].toString();
            ioClassification.addGroup(ioType,groupName);
            QJsonArray purposes = group["list"].toArray();
            for(const auto &purposeOb:purposes) {
                QJsonObject ob = purposeOb.toObject();
                QString purpose = ob["data"].toString();
                int code = ob["code"].toInt();
                ioClassification.addIOPurposeToGroup(ioType,groupName,purpose,code);
            }
        }
    }
    // add di and do purposes to dio type
    createDIOPurposesFromDiAndDO(ioClassification);

    for(const auto &type:aiTypes) {
        QString aiString = type.first;
        IOClassification::AI_TYPE aiType = type.second;
        QJsonArray codes = json[aiString].toArray();
        std::set<int> data;
        for(const auto &code:codes) {
            int aiCode = code.toInt();
            data.insert(aiCode);
        }
        ioClassification.addAIType(aiType,data);
    }
}

void JSONIOClassificationReader::createDIOPurposesFromDiAndDO(IOClassification &ioClassification)
{
    QStringList diGroups = ioClassification.getGroupNamesByType(IOBase::IOTYPES::DISCRETE_INPUT);
    QStringList doGroups = ioClassification.getGroupNamesByType(IOBase::IOTYPES::DISCRETE_OUTPUT);
    for(QString diGroup:diGroups) {
        ioClassification.addGroup(IOBase::IOTYPES::DISCRETE_INPUT_OUTPUT,diGroup);
        auto purposes = ioClassification.getIOPurposesByGroup(IOBase::IOTYPES::DISCRETE_INPUT,diGroup);
        for(const auto &purpose:purposes) {
            QString name = purpose.first;
            int code = purpose.second;
            ioClassification.addIOPurposeToGroup(IOBase::IOTYPES::DISCRETE_INPUT_OUTPUT,diGroup,name,code);
        }
    }
    for(QString doGroup:doGroups) {
        ioClassification.addGroup(IOBase::IOTYPES::DISCRETE_INPUT_OUTPUT,doGroup);
        auto purposes = ioClassification.getIOPurposesByGroup(IOBase::IOTYPES::DISCRETE_OUTPUT,doGroup);
        for(const auto &purpose:purposes) {
            QString name = purpose.first;
            int code = purpose.second;
            ioClassification.addIOPurposeToGroup(IOBase::IOTYPES::DISCRETE_INPUT_OUTPUT,doGroup,name,code);
        }
    }
}

void JSONIOClassificationReader::readIOClassification(const QString &fName, IOClassification &ioClassification)
{
    QFile loadFile(fName);

    if (loadFile.open(QIODevice::ReadOnly)) {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        read(loadDoc.object(),ioClassification);
    }else {
        QString message = "Couldn't open file " + fName;
        qWarning(message.toStdString().c_str());
    }
}
