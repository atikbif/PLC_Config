#ifndef AICONFIG_H
#define AICONFIG_H

#include <map>
#include <QString>
#include "ioclassification.h"
#include <array>
#include <map>

#include <QDebug>

class AIConfig
{
    static const int ai_cnt = 8;
    static const int di_cnt = 24;
    // карта памяти заводских установок с настройками аналоговых входов (индекс: номер входа - 1, значение адрес уставки)

    static const std::array<int,ai_cnt> sens_level_4ma_address;
    static const std::array<int,ai_cnt> sens_level_20ma_address;
    static const std::array<int,ai_cnt> sens_offset_address;
    static const std::array<int,ai_cnt> sens_average_time_address;

    static const std::array<int,ai_cnt> damage_min_address;
    static const std::array<int,ai_cnt> damage_max_address;
    static const std::array<int,ai_cnt> damage_hysteresis_min_address;
    static const std::array<int,ai_cnt> damage_hysteresis_max_address;
    static const std::array<int,ai_cnt> damage_filter_time_address;

    static const std::array<int,di_cnt> di_sens_filter_time_address;
    static const std::array<int,di_cnt> di_sens_type_address;

    static double transformEE(int eeValue,double coeff=1.0,bool signFlag=false);
public:

    template<typename Array>
    static int getParam(int num, const Array &addrArray) {
        if((num<1)||(num>ai_cnt)) return 0;
        int addr = addrArray.at(num-1);
        int eeValue = 0;
        auto it = eeBufMap.find(addr);
        if(it!=eeBufMap.end())  {
            eeValue = it->second;
        }else eeValue = 0;
        return eeValue;
    }

    template<typename Array>
    static int getDIParam(int num, const Array &addrArray) {
        if((num<1)||(num>di_cnt)) return 0;
        int addr = addrArray.at(num-1);
        int eeValue = 0;
        auto it = eeBufMap.find(addr);
        if(it!=eeBufMap.end())  {
            eeValue = it->second;
        }else eeValue = 0;
        return eeValue;
    }

    static std::map<int,int> eeBufMap;
    static void readEEMap(const QString &fName);
    static void writeEEMap(const QString &fName);
    struct Sensor {
        double level_4ma;
        double level_20ma;
        double offset;
        double average_time;
    };

    struct Damage {
        double min;
        double max;
        double hysteresis_min;
        double hysteresis_max;
        double filter_time;
    };

    struct DISensor {
        explicit DISensor(int diNum, bool readFromMap=true);
        double filter_time;
        bool type;
        int getNum() const {return num;}
        void toMap();
    private:
        int num;
    };

    struct State {
        explicit State(int aiNum,IOClassification::AI_TYPE aiType = IOClassification::AI_TYPE::UNKNOWN,bool readFromMap = true);
        Sensor sensor;
        Damage damage;
        int getNum() const {return num;}
        IOClassification::AI_TYPE getType() const {return type;}
        void toMap(); // формирует область EEPROM (адрес/значение), соответствующую настройкам датчика
    private:
        void temperatureToMap();
        void pressureToMap();
        void frequencyToMap();
        void levelToMap();
        void unknownToMap();
    private:
        int num;
        IOClassification::AI_TYPE type;
    };
};

#endif // AICONFIG_H
