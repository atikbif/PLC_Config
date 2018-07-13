#include "dialogdiconfig.h"
#include "ui_dialogdiconfig.h"

DialogDIConfig::DialogDIConfig(const AIConfig::DISensor &state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDIConfig)
{
    ui->setupUi(this);

    ui->doubleSpinBoxFilter->setValue(state.filter_time);
    ui->comboBox->setCurrentIndex(state.type?0:1);
    num = state.getNum();
}

AIConfig::DISensor DialogDIConfig::getDIState() const
{
    AIConfig::DISensor state(num);
    state.filter_time = ui->doubleSpinBoxFilter->value();
    state.type = ui->comboBox->currentIndex()?false:true;
    return state;
}

DialogDIConfig::~DialogDIConfig()
{
    delete ui;
}
