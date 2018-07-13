#ifndef SFRAM_H
#define SFRAM_H

// класс работы с системной областью FRAM (чтение/запись настроек)

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QMutex>
#include <utility>
#include <QVector>

class sFram : public QObject
{
    Q_OBJECT
    QMutex mutex;
    bool stopCmd;
    static const int reqLength = 64;
    void controllerReset(QSerialPort &port);
    bool testControllerReady(void); // проверка готовности контроллера к чтению/записи настроек
    bool writeOneRequest(QSerialPort &port,int reqAddr,const QByteArray &data);
    bool readOneRequest(QSerialPort &port,int reqAddr,QByteArray &data);
public:
    explicit sFram(QObject *parent = 0);
    ~sFram();

signals:
    void writeFinished(void);
    void readFinished(QVector<std::pair<int,int>> data);
    void error(QString message);
    void percentUpdate(float value);
public slots:
    void startWrite(QVector<std::pair<int,int>> data);
    void startRead(QVector<int> addr);
    void stopProcess(void);
};

#endif // SFRAM_H
