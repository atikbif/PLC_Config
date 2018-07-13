#ifndef STANDARDCONFIGREADER_H
#define STANDARDCONFIGREADER_H

#include "standardconfigstorage.h"

// базовый интерфейс чтения конфигурации из файла

class StandardConfigReader {
public:
    virtual StandardConfigStorage readStandardConfig(const QString &fName)=0;
};

#endif // STANDARDCONFIGREADER_H
