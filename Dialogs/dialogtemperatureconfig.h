#ifndef DIALOGTEMPERATURECONFIG_H
#define DIALOGTEMPERATURECONFIG_H

#include <QDialog>
#include "InputOutput/aiconfig.h"

namespace Ui {
class DialogTemperatureConfig;
}

class DialogTemperatureConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTemperatureConfig(const AIConfig::State &state, QWidget *parent = 0);
    AIConfig::State getAIState() const;
    ~DialogTemperatureConfig();

private:
    Ui::DialogTemperatureConfig *ui;
    int num;
    IOClassification::AI_TYPE type;
};

#endif // DIALOGTEMPERATURECONFIG_H
