#ifndef DIALOGDICONFIG_H
#define DIALOGDICONFIG_H

#include <QDialog>
#include "InputOutput/aiconfig.h"

namespace Ui {
class DialogDIConfig;
}

class DialogDIConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDIConfig(const AIConfig::DISensor &state, QWidget *parent = 0);
    AIConfig::DISensor getDIState() const;
    ~DialogDIConfig();

private:
    Ui::DialogDIConfig *ui;
    int num;
};

#endif // DIALOGDICONFIG_H
