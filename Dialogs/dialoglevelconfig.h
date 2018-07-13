#ifndef DIALOGLEVELCONFIG_H
#define DIALOGLEVELCONFIG_H

#include <QDialog>
#include "InputOutput/aiconfig.h"

namespace Ui {
class DialogLevelConfig;
}

class DialogLevelConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLevelConfig(const AIConfig::State &state, QWidget *parent = 0);
    AIConfig::State getAIState() const;
    ~DialogLevelConfig();

private:
    Ui::DialogLevelConfig *ui;
    int num;
    IOClassification::AI_TYPE type;
};

#endif // DIALOGLEVELCONFIG_H
