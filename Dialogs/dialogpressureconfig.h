#ifndef DIALOGPRESSURECONFIG_H
#define DIALOGPRESSURECONFIG_H

#include <QDialog>
#include "InputOutput/aiconfig.h"

namespace Ui {
class DialogPressureConfig;
}

class DialogPressureConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPressureConfig(const AIConfig::State &state, QWidget *parent = 0);
    AIConfig::State getAIState() const;
    ~DialogPressureConfig();

private:
    Ui::DialogPressureConfig *ui;
    int num;
    IOClassification::AI_TYPE type;
};

#endif // DIALOGPRESSURECONFIG_H
