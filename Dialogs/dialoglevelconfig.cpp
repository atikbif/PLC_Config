#include "dialoglevelconfig.h"
#include "ui_dialoglevelconfig.h"

DialogLevelConfig::DialogLevelConfig(const AIConfig::State &state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLevelConfig)
{
    ui->setupUi(this);
    ui->doubleSpinBox4ma->setValue(state.sensor.level_4ma);
    ui->doubleSpinBox20ma->setValue(state.sensor.level_20ma);
    ui->doubleSpinBoxAverageTime->setValue(state.sensor.average_time);
    ui->doubleSpinBoxOffset->setValue(state.sensor.offset);
    ui->doubleSpinBoxAvarMin->setValue(state.damage.min);
    ui->doubleSpinBoxAvarMax->setValue(state.damage.max);
    ui->doubleSpinBoxHysteresisMin->setValue(state.damage.hysteresis_min);
    ui->doubleSpinBoxHysteresisMax->setValue(state.damage.hysteresis_max);
    ui->doubleSpinBoxFilterTime->setValue(state.damage.filter_time);
    num = state.getNum();
    type = state.getType();
}

AIConfig::State DialogLevelConfig::getAIState() const
{
    AIConfig::State state(num,type,false);
    state.sensor.level_4ma = ui->doubleSpinBox4ma->value();
    state.sensor.level_20ma = ui->doubleSpinBox20ma->value();
    state.sensor.average_time = ui->doubleSpinBoxAverageTime->value();
    state.sensor.offset = ui->doubleSpinBoxOffset->value();
    state.damage.min = ui->doubleSpinBoxAvarMin->value();
    state.damage.max = ui->doubleSpinBoxAvarMax->value();
    state.damage.hysteresis_min = ui->doubleSpinBoxHysteresisMin->value();
    state.damage.hysteresis_max = ui->doubleSpinBoxHysteresisMax->value();
    state.damage.filter_time = ui->doubleSpinBoxFilterTime->value();
    return state;
}

DialogLevelConfig::~DialogLevelConfig()
{
    delete ui;
}
