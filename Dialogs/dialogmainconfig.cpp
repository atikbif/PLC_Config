#include "dialogmainconfig.h"
#include "ui_dialogmainconfig.h"

DialogMainConfig::DialogMainConfig(bool pultState, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMainConfig)
{
    ui->setupUi(this);
    if(pultState) ui->radioButtonPultOn->setChecked(true);
    else ui->radioButtonPultOff->setChecked(true);
}

bool DialogMainConfig::getPultState()
{
    return ui->radioButtonPultOn->isChecked();
}

DialogMainConfig::~DialogMainConfig()
{
    delete ui;
}
