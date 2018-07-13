#include "plcviewstorage.h"

PLCViewStorage::PLCViewStorage(std::unique_ptr<IPLCViewReader> reader, const QString &fName)
{
    plcViews = reader->readData(fName);
}

QStringList PLCViewStorage::getPLCViewNames() const
{
    QStringList names;
    for(const auto& plc:plcViews) {
        names.append(plc.getName());
    }
    return names;
}

PLCView PLCViewStorage::getPLCViewByName(const QString &name, bool *res)
{
    if(res!=nullptr) *res = false;
    for(const auto& plc:plcViews) {
        if(plc.getName()==name) {
            if(res!=nullptr) *res = true;
            return plc;
        }
    }
    return PLCView();
}
