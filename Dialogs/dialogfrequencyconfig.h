#ifndef DIALOGFREQUENCYCONFIG_H
#define DIALOGFREQUENCYCONFIG_H

#include <QDialog>
#include "InputOutput/aiconfig.h"

namespace Ui {
class DialogFrequencyConfig;
}

class DialogFrequencyConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFrequencyConfig(const AIConfig::State &state, QWidget *parent = 0);
    AIConfig::State getAIState() const;
    ~DialogFrequencyConfig();

private:
    Ui::DialogFrequencyConfig *ui;
    int num;
    IOClassification::AI_TYPE type;
};

#endif // DIALOGFREQUENCYCONFIG_H
