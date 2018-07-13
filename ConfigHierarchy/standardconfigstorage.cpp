#include "standardconfigstorage.h"

StandardConfigStorage::StandardConfigStorage()
{

}

StandardConfigStorage::Type StandardConfigStorage::createType(const QString &name, int num)
{
    Type t;
    t.name = name;
    t.num = num;
    return t;
}

StandardConfigStorage::Config StandardConfigStorage::createConfig(const QString &name, const QString &file, const QString &comment, const QString &plc)
{
    Config conf;
    conf.name = name;
    conf.file = file;
    conf.comment = comment;
    conf.plc = plc;
    return conf;
}

StandardConfigStorage::Device StandardConfigStorage::createDevice(const QString &name)
{
    Device dev;
    dev.name = name;
    return dev;
}

void StandardConfigStorage::addConfigToDevice(const StandardConfigStorage::Config &conf, StandardConfigStorage::Device &dev)
{
    dev.configs.append(conf);
}

void StandardConfigStorage::addDeviceToType(const StandardConfigStorage::Device &dev, StandardConfigStorage::Type &t)
{
    t.devices.append(dev);
}

void StandardConfigStorage::addTypeToConfig(const StandardConfigStorage::Type &t)
{
    types.append(t);
}

QVector<StandardConfigStorage::Type> StandardConfigStorage::getTypes() const
{
    return types;
}
