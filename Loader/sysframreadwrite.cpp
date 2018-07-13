#include "sysframreadwrite.h"
#include "ui_sysframreadwrite.h"
#include <QMetaType>

SysFramReadWrite::SysFramReadWrite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysFramReadWrite)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
    ui->progressBar->setValue(0);

    qRegisterMetaType<QVector<std::pair<int,int>>>("PLC_Conf");
    qRegisterMetaType<QVector<int>>("PLC_Conf_Addr");

    loader = new sFram();
    loader->moveToThread(&sFramThread);
    connect(&sFramThread,SIGNAL(finished()),loader,SLOT(deleteLater()));
    connect(loader,SIGNAL(percentUpdate(float)),this,SLOT(percentUpdate(float)));
    connect(loader,SIGNAL(writeFinished()),this,SLOT(writeFinished()));
    connect(loader,SIGNAL(readFinished(QVector<std::pair<int,int> >)),this,SLOT(readFinished(QVector<std::pair<int,int> >)));
    connect(loader,SIGNAL(error(QString)),this,SLOT(error(QString)));
    connect(this,SIGNAL(startWrite(QVector<std::pair<int,int> >)),loader,SLOT(startWrite(QVector<std::pair<int,int> >)));
    connect(this,SIGNAL(startRead(QVector<int>)),loader,SLOT(startRead(QVector<int>)));
    sFramThread.start();
}

SysFramReadWrite::~SysFramReadWrite()
{
    loader->stopProcess();
    sFramThread.quit();
    sFramThread.wait();
    delete ui;
}

void SysFramReadWrite::startWriteProcess(QVector<std::pair<int, int> > data)
{
    emit startWrite(data);
}

void SysFramReadWrite::startReadProcess(QVector<int> addr)
{
    emit startRead(addr);
}

void SysFramReadWrite::writeFinished()
{
    accept();
}

void SysFramReadWrite::readFinished(QVector<std::pair<int, int> > data)
{
    emit readOK(data);
    accept();
}

void SysFramReadWrite::error(QString message)
{
    ui->label->setVisible(true);
    ui->label->setText(message);
    repaint();
}

void SysFramReadWrite::percentUpdate(float value)
{
    ui->progressBar->setValue(value);
}
