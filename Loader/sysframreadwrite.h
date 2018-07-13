#ifndef SYSFRAMREADWRITE_H
#define SYSFRAMREADWRITE_H

// GUI работы с FRAM памятью в отдельном потоке

#include <QDialog>
#include <QThread>
#include "sfram.h"

namespace Ui {
class SysFramReadWrite;
}

class SysFramReadWrite : public QDialog
{
    Q_OBJECT
    QThread sFramThread;
    sFram* loader;
public:
    explicit SysFramReadWrite(QWidget *parent = 0);
    ~SysFramReadWrite();
signals:
    void startWrite(QVector<std::pair<int,int>>);
    void startRead(QVector<int>);

    void readOK(QVector<std::pair<int,int>> inpData);
public slots:
    void startWriteProcess(QVector<std::pair<int,int>> data);
    void startReadProcess(QVector<int> addr);
private slots:
    void writeFinished(void);
    void readFinished(QVector<std::pair<int,int>> data);
    void error(QString message);
    void percentUpdate(float value);

private:
    Ui::SysFramReadWrite *ui;
};

#endif // SYSFRAMREADWRITE_H
