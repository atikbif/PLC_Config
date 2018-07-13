#include "dialogdeviceselect.h"
#include "ui_dialogdeviceselect.h"


DialogDeviceSelect::DialogDeviceSelect(const StandardConfigStorage &storage, int typeNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDeviceSelect)
{
    ui->setupUi(this);
    auto types = storage.getTypes();
    for(const auto &type:types) {
        if(type.num == typeNum) {
            const auto &devices = type.devices;
            confDevices = devices;
            for(const auto &device:devices) {
                ui->listWidgetDevices->addItem(device.name);
            }
            ui->listWidgetDevices->setCurrentRow(0);
            break;
        }
    }
}

DialogDeviceSelect::~DialogDeviceSelect()
{
    delete ui;
}

void DialogDeviceSelect::on_listWidgetDevices_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    if(current==nullptr) return;
    ui->listWidgetConfigs->clear();
    for(const auto &device:confDevices) {
        if(device.name==current->text()) {
            const auto configs = device.configs;
            for(const auto &config:configs) {
                ui->listWidgetConfigs->addItem(config.name);
            }
            if(configs.count()) {
                ui->listWidgetConfigs->setCurrentRow(0);
                ui->textEditComment->setText(configs.at(0).comment);
                confFile = configs.at(0).file;
                comment = configs.at(0).comment;
                plc = configs.at(0).plc;
            }
            currentConfigs = configs;
        }
    }
}

void DialogDeviceSelect::on_listWidgetConfigs_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    if(current==nullptr) return;
    for(const auto &conf:currentConfigs) {
        if(conf.name==current->text()) {
            ui->textEditComment->setText(conf.comment);
            confFile = conf.file;
            comment = conf.comment;
            plc = conf.plc;
        }
    }
}
