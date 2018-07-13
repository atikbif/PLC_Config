#ifndef IIOCLASSIFICATIONREADER_H
#define IIOCLASSIFICATIONREADER_H

#include <QString>

#include "iobase.h"
#include "ioclassification.h"


class IIOClassificationReader {
public:
    virtual void readIOClassification(const QString &fName, IOClassification &ioClassification) = 0;
};

#endif // IIOCLASSIFICATIONREADER_H
