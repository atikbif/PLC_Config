#ifndef DIALOGDEFAULTAICONFIG_H
#define DIALOGDEFAULTAICONFIG_H

#include <QDialog>
#include "InputOutput/aiconfig.h"

namespace Ui {
class DialogDefaultAIConfig;
}

class DialogDefaultAIConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDefaultAIConfig(const AIConfig::State &state, QWidget *parent = 0);
    AIConfig::State getAIState() const;
    ~DialogDefaultAIConfig();

private:
    Ui::DialogDefaultAIConfig *ui;
    int num;
    IOClassification::AI_TYPE type;
};

#endif // DIALOGDEFAULTAICONFIG_H
