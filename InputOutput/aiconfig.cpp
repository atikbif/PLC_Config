#include "aiconfig.h"
#include <algorithm>
#include <math.h>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

std::map<int,int> AIConfig::eeBufMap = {};

const std::array<int,AIConfig::ai_cnt> AIConfig::sens_level_4ma_address = {664,666,668,670,672,674,676,678};
const std::array<int,AIConfig::ai_cnt> AIConfig::sens_level_20ma_address = {680,682,684,686,688,690,692,694};
const std::array<int,AIConfig::ai_cnt> AIConfig::sens_offset_address = {696,698,700,702,704,706,708,710};
const std::array<int,AIConfig::ai_cnt> AIConfig::sens_average_time_address = {712,714,716,718,720,722,724,726};

const std::array<int,AIConfig::ai_cnt> AIConfig::damage_min_address = {744,746,748,750,752,754,756,758};
const std::array<int,AIConfig::ai_cnt> AIConfig::damage_max_address = {760,762,764,766,768,770,772,774};
const std::array<int,AIConfig::ai_cnt> AIConfig::damage_hysteresis_min_address = {776,778,780,782,784,786,788,790};
const std::array<int,AIConfig::ai_cnt> AIConfig::damage_hysteresis_max_address = {792,794,796,798,800,802,804,806};
const std::array<int,AIConfig::ai_cnt> AIConfig::damage_filter_time_address = {728,730,732,734,736,738,740,742};

const std::array<int,AIConfig::di_cnt> AIConfig::di_sens_filter_time_address = {880,882,884,886,888,890,892,894,896,898,900,902,904,906,908,910,912,914,916,918,920,922,924,926};
const std::array<int,AIConfig::di_cnt> AIConfig::di_sens_type_address = {928,930,932,934,936,938,940,942,944,946,948,950,952,954,956,958,960,962,964,966,968,970,972,974};

double AIConfig::transformEE(int eeValue, double coeff, bool signFlag)
{
    if(signFlag) eeValue = static_cast<qint16>((quint16)eeValue);
    double res = eeValue*coeff;
    return res;
}

void AIConfig::readEEMap(const QString &fName)
{
    eeBufMap.clear();
    QFile loadFile(fName);

    if (loadFile.open(QIODevice::ReadOnly)) {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        auto json = loadDoc.object();
        auto values = json["map"].toArray();
        for(const auto &value:values) {
            auto ob = value.toObject();
            int eeNum = ob["ee_num"].toInt();
            int eeValue = ob["value"].toInt();
            eeBufMap[eeNum] = eeValue;
        }
    }else {
        QString message = "Couldn't open file " + fName;
        qWarning(message.toStdString().c_str());
    }
}

void AIConfig::writeEEMap(const QString &fName)
{
    QFile saveFile(fName);
    if (saveFile.open(QIODevice::WriteOnly)) {

        QJsonObject saveObject;
        saveObject["version"] = 1;

        QJsonArray eeList;
        for(const auto &ee:eeBufMap) {
                int eeNum = ee.first;
                int eeValue = ee.second;
                QJsonObject ob;
                ob["ee_num"] = eeNum;
                ob["value"] = eeValue;
                eeList.append(ob);
        }
        saveObject["map"] = eeList;
        QJsonDocument saveDoc(saveObject);
        saveFile.write(saveDoc.toJson());
    }else {
        QString message = "Couldn't open file";
        qWarning(message.toStdString().c_str());
    }
}


AIConfig::State::State(int aiNum, IOClassification::AI_TYPE aiType, bool readFromMap): num(aiNum),type(aiType)
{
    // чтение настроек из буфера карты FRAM памяти
    // и формирование состояния конфигурации входа

    using TYPE = IOClassification::AI_TYPE;

    if(readFromMap)
    {

        int level_4ma = AIConfig::getParam(num,sens_level_4ma_address);
        int level_20ma = AIConfig::getParam(num,sens_level_20ma_address);
        int offsetValue = AIConfig::getParam(num,sens_offset_address);
        int averageTime = AIConfig::getParam(num,sens_average_time_address);
        int damageMin = AIConfig::getParam(num,damage_min_address);
        int damageMax = AIConfig::getParam(num,damage_max_address);
        int hysteresisMin = AIConfig::getParam(num,damage_hysteresis_min_address);
        int hysteresisMax = AIConfig::getParam(num,damage_hysteresis_max_address);
        int filterTime = AIConfig::getParam(num,damage_filter_time_address);

        switch(aiType)
        {
            case TYPE::TEMPERATURE:
            case TYPE::LEVEL:
            case TYPE::FREQUENCY:
            case TYPE::UNKNOWN:
                sensor.level_4ma = AIConfig::transformEE(level_4ma,1,true);
                sensor.level_20ma = AIConfig::transformEE(level_20ma,1,true);
                sensor.offset = AIConfig::transformEE(offsetValue,1,true);
                sensor.average_time = AIConfig::transformEE(averageTime,0.1);
                damage.min = AIConfig::transformEE(damageMin,1,true);
                damage.max = AIConfig::transformEE(damageMax,1,true);
                damage.hysteresis_min = AIConfig::transformEE(hysteresisMin,1,true);
                damage.hysteresis_max = AIConfig::transformEE(hysteresisMax,1,true);
                damage.filter_time = AIConfig::transformEE(filterTime,0.1);
                break;
            case TYPE::PRESSURE:
                sensor.level_4ma = AIConfig::transformEE(level_4ma,0.1);
                sensor.level_20ma = AIConfig::transformEE(level_20ma,0.1);
                sensor.offset = AIConfig::transformEE(offsetValue,0.1,true);
                sensor.average_time = AIConfig::transformEE(averageTime,0.1);
                damage.min = AIConfig::transformEE(damageMin,0.1);
                damage.max = AIConfig::transformEE(damageMax,0.1);
                damage.hysteresis_min = AIConfig::transformEE(hysteresisMin,0.1);
                damage.hysteresis_max = AIConfig::transformEE(hysteresisMax,0.1);
                damage.filter_time = AIConfig::transformEE(filterTime,0.1);
                break;
        }
    }
}

void AIConfig::State::toMap()
{
    if(type==IOClassification::AI_TYPE::TEMPERATURE) temperatureToMap();
    else if(type==IOClassification::AI_TYPE::PRESSURE) pressureToMap();
    else if(type==IOClassification::AI_TYPE::FREQUENCY) frequencyToMap();
    else if(type==IOClassification::AI_TYPE::LEVEL) levelToMap();
    else if(type==IOClassification::AI_TYPE::UNKNOWN) unknownToMap();
}

void AIConfig::State::temperatureToMap()
{
    AIConfig::State::unknownToMap();
}

void AIConfig::State::pressureToMap()
{
    if((num<1)||(num>ai_cnt)) return;
    int addr = sens_level_4ma_address.at(num-1);
    int eeValue = static_cast<quint16>(qint16(round(sensor.level_4ma*10)));
    eeBufMap[addr] = eeValue;

    addr = sens_level_20ma_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(sensor.level_20ma*10)));
    eeBufMap[addr] = eeValue;

    addr = sens_offset_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(sensor.offset*10)));
    eeBufMap[addr] = eeValue;

    addr = sens_average_time_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(sensor.average_time*10)));
    eeBufMap[addr] = eeValue;

    addr = damage_min_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.min*10)));
    eeBufMap[addr] = eeValue;

    addr = damage_max_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.max*10)));
    eeBufMap[addr] = eeValue;

    addr = damage_hysteresis_min_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.hysteresis_min*10)));
    eeBufMap[addr] = eeValue;

    addr = damage_hysteresis_max_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.hysteresis_max*10)));
    eeBufMap[addr] = eeValue;

    addr = damage_filter_time_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.filter_time*10)));
    eeBufMap[addr] = eeValue;
}

void AIConfig::State::frequencyToMap()
{
    AIConfig::State::unknownToMap();
}

void AIConfig::State::levelToMap()
{
    AIConfig::State::unknownToMap();
}

void AIConfig::State::unknownToMap()
{
    if((num<1)||(num>ai_cnt)) return;
    int addr = sens_level_4ma_address.at(num-1);
    int eeValue = static_cast<quint16>(qint16(round(sensor.level_4ma)));
    eeBufMap[addr] = eeValue;

    addr = sens_level_20ma_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(sensor.level_20ma)));
    eeBufMap[addr] = eeValue;

    addr = sens_offset_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(sensor.offset)));
    eeBufMap[addr] = eeValue;

    addr = sens_average_time_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(sensor.average_time*10)));
    eeBufMap[addr] = eeValue;

    addr = damage_min_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.min)));
    eeBufMap[addr] = eeValue;

    addr = damage_max_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.max)));
    eeBufMap[addr] = eeValue;

    addr = damage_hysteresis_min_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.hysteresis_min)));
    eeBufMap[addr] = eeValue;

    addr = damage_hysteresis_max_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.hysteresis_max)));
    eeBufMap[addr] = eeValue;

    addr = damage_filter_time_address.at(num-1);
    eeValue = static_cast<quint16>(qint16(round(damage.filter_time*10)));
    eeBufMap[addr] = eeValue;
}

AIConfig::DISensor::DISensor(int diNum, bool readFromMap):num(diNum)
{
    if(readFromMap)
    {
        filter_time = AIConfig::transformEE(AIConfig::getDIParam(num,di_sens_filter_time_address),0.1);
        type = AIConfig::getDIParam(num,di_sens_type_address)?true:false;
    }
}

void AIConfig::DISensor::toMap()
{
    if((num<1)||(num>di_cnt)) return;
    int addr = di_sens_filter_time_address.at(num-1);
    int eeValue = static_cast<quint16>(qint16(round(filter_time*10)));
    eeBufMap[addr] = eeValue;

    addr = di_sens_type_address.at(num-1);
    eeValue = type?1:0;
    eeBufMap[addr] = eeValue;
}
