#include "inputoutput.h"

const QString InputOutput::defaultIOState = "Не используется";

InputOutput::InputOutput():IOBase()
{
    code = 0;
}

InputOutput::InputOutput(const IOBase &io)
{
    setPinNumber(io.getPinNumber());
    setInputNumber(io.getInputNumber());
    setOutputNumber(io.getOutputNumber());
    setType(io.getType());
    code = 0;
}
