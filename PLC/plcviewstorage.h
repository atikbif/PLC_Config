#ifndef PLCVIEWSTORAGE_H
#define PLCVIEWSTORAGE_H

#include <QVector>
#include "InputOutput/inputoutput.h"
#include "ioview.h"
#include "plcview.h"
#include "iplcviewreader.h"
#include <memory>

class PLCViewStorage
{
    QVector<PLCView> plcViews;
public:
    PLCViewStorage(std::unique_ptr<IPLCViewReader> reader, const QString &fName);
    QStringList getPLCViewNames() const;
    PLCView getPLCViewByName(const QString &name,bool *res=nullptr);
    int count() const {return plcViews.count();}
};

#endif // PLCVIEWSTORAGE_H
