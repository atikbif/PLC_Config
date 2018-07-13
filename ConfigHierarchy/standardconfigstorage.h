#ifndef STANDARDCONFIGSTORAGE_H
#define STANDARDCONFIGSTORAGE_H

#include <QStringList>
#include <QVector>

// иерархия проекта:
// тип проекта (котельные, вентиляция, регуляторы, ...)
// для каждого типа список устройств (менеджер котельной, контроллер безопасности, контроллер каскада, ...)
// внутри каждого устройства доступны различные конфигурации (отличаются функциями входов/выходов контроллера)

class StandardConfigStorage
{

public:
    // конфигурация контроллера
    struct Config {
        QString name; // название конфигурации
        QString file; // файл с описанием конфигурации
        QString comment; // расширенное описание
        QString plc;    // марка контроллера
        QString baseType;   // к какому типу относится конфигурация
        QString baseDevice; // к какому устройству относится конфигурация
        QString defaultEE; // имя файла с типовой конфигурацией заводских установок
    };

    // устройство
    struct Device {
        QString name;
        QVector<Config> configs; // список доступных для устройства конфигураций
    };

    struct Type {
        int num;    // индекс типа
        QString name;
        QVector<Device> devices;    // список доступных устройств
    };

private:
    QVector<Type> types;

public:
    StandardConfigStorage();
    Type createType(const QString &name, int num);
    Config createConfig(const QString &name, const QString &file, const QString &comment, const QString &plc);
    Device createDevice(const QString &name);
    void addConfigToDevice(const Config &conf, Device &dev);
    void addDeviceToType(const Device &dev, Type &t);
    void addTypeToConfig(const Type &t);
    QVector<Type> getTypes() const;
};

#endif // STANDARDCONFIGSTORAGE_H
