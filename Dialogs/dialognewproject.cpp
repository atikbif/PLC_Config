#include "dialognewproject.h"
#include "ui_dialognewproject.h"

DialogNewProject::DialogNewProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewProject)
{
    ui->setupUi(this);
}

DialogNewProject::~DialogNewProject()
{
    delete ui;
}

void DialogNewProject::on_pushButtonBoil_clicked()
{
    dev = DeviceType::BOIL;
}

void DialogNewProject::on_pushButtonVent_clicked()
{
    dev = DeviceType::VENT;
}
