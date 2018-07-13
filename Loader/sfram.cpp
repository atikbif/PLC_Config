#include "sfram.h"
#include "AutoSearch/detectedcontroller.h"
#include <QThread>
#include "Protocols/commandinterface.h"
#include "Protocols/asciidecorator.h"
#include "Protocols/rk.h"

using namespace RkProtocol;

void sFram::controllerReset(QSerialPort &port)
{
    CommandInterface* cmd = new ResetController();
    DetectedController* plc = &DetectedController::Instance();
    Request req;
    req.setNetAddress(plc->getNetAddress());
    if(plc->getAsciiMode()) cmd = new AsciiDecorator(cmd);
    if(port.open(QSerialPort::ReadWrite)){
        cmd->execute(req,port);
        port.close();
    }
    delete cmd;
}

bool sFram::testControllerReady()
{
    DetectedController* plc = &DetectedController::Instance();
    // проверка результатов последнего поиска
    if(plc->getUartName().isEmpty()) {
        emit error("Не задано имя COM порта");
        return false;
    }
    if(plc->getBootMode()){
        emit error("Контроллер ожидает загрузки программы. Чтение/запись настроек невозможны.");
        return false;
    }
    return true;
}

bool sFram::writeOneRequest(QSerialPort &port, int reqAddr, const QByteArray &data)
{
    DetectedController* plc = &DetectedController::Instance();
    CommandInterface* cmd = new WriteFram();
    if(plc->getAsciiMode()) cmd = new AsciiDecorator(cmd);
    Request req;
    req.setNetAddress(0);
    req.setDataNumber(data.count());
    req.setMemAddress(reqAddr);
    req.setWrData(data);
    bool errFlag = true;
    for(int j=0;j<3;j++) {
        if(cmd->execute(req,port)==true) {errFlag=false;break;}
        mutex.lock();
        if(stopCmd) {port.close();mutex.unlock();return !errFlag;}
        mutex.unlock();
    }
    if(errFlag){
        emit error("Ошибка записи FRAM памяти контроллера");
        delete cmd;
        cmd = nullptr;
    }
    delete cmd;
    return !errFlag;
}

bool sFram::readOneRequest(QSerialPort &port, int reqAddr, QByteArray &data)
{
    DetectedController* plc = &DetectedController::Instance();
    CommandInterface* cmd = new ReadFram();
    if(plc->getAsciiMode()) cmd = new AsciiDecorator(cmd);
    Request req;
    req.setNetAddress(0);
    req.setDataNumber(data.count());
    req.setMemAddress(reqAddr);
    bool errFlag = true;
    for(int j=0;j<3;j++) {
        if(cmd->execute(req,port)==true) {errFlag=false;break;}
        mutex.lock();
        if(stopCmd) {port.close();mutex.unlock();return !errFlag;}
        mutex.unlock();
    }
    if(errFlag){
        emit error("Ошибка чтения FRAM памяти контроллера");
        delete cmd;
        cmd = nullptr;
    }else {
        data = req.getRdData();
    }
    delete cmd;
    return !errFlag;
}

sFram::sFram(QObject *parent) : QObject(parent),stopCmd(false)
{

}

sFram::~sFram()
{

}

void sFram::startWrite(QVector<std::pair<int,int>> data)
{
    DetectedController* plc = &DetectedController::Instance();
    if(!testControllerReady()) return;
    QSerialPort port(plc->getUartName());
    port.setBaudRate(plc->getBaudrate());
    if(port.open(QSerialPort::ReadWrite)) {

        float stepPercent = 0;
        stepPercent = 100/(float)data.count();
        if(data.count()) {
            float curPercent = 0;
            QByteArray wrData;
            int reqAddr = 0;
            int prevAddr = 0;
            int cnt = 0;
            bool newRequest = true;
            for(const auto &pair:data) {
                cnt++;
                if(newRequest) {
                    reqAddr = pair.first;
                    wrData.clear();
                    wrData.append(pair.second);
                    newRequest = false;
                }else {
                    if(pair.first-prevAddr==1) {
                        wrData.append(pair.second);
                        if((wrData.count()>=reqLength)||(cnt==data.count())) {
                            // отправить запрос

                            newRequest = true;
                            if(writeOneRequest(port,reqAddr,wrData)) {
                                curPercent+=cnt*stepPercent;
                                emit percentUpdate(curPercent);
                            }else break;
                        }
                    }else {
                        // отправить запрос

                        if(writeOneRequest(port,reqAddr,wrData)) {
                            curPercent+=cnt*stepPercent;
                            emit percentUpdate(curPercent);
                        }else break;

                        reqAddr = pair.first;
                        wrData.clear();
                        wrData.append(pair.second);

                        if(cnt==data.count()) {
                            // отправить запрос
                            writeOneRequest(port,reqAddr,wrData);
                            curPercent+=cnt*stepPercent;
                            emit percentUpdate(curPercent);
                        }
                    }
                }
                prevAddr = pair.first;
            }
        }
        port.close();
    }else emit error("Ошибка открытия порта "+plc->getUartName());

    // пересброс контроллера
    controllerReset(port);
    emit writeFinished();

}

void sFram::startRead(QVector<int> addr)
{
    QVector<std::pair<int,int>> result;

    DetectedController* plc = &DetectedController::Instance();
    if(!testControllerReady()) return;
    QSerialPort port(plc->getUartName());
    port.setBaudRate(plc->getBaudrate());
    if(port.open(QSerialPort::ReadWrite)) {

        float stepPercent = 0;

        stepPercent = 100/(float)addr.count();

        if(addr.count()) {
            float curPercent = 0;
            QByteArray rdData;
            int reqAddr = 0;
            int prevAddr = 0;
            int cnt = 0;
            bool newRequest = true;
            for(int ad:addr) {
                cnt++;
                if(newRequest) {
                    reqAddr = ad;
                    rdData.clear();
                    rdData.append('\0');
                    newRequest = false;
                }else {
                    if(ad-prevAddr==1) {
                        rdData.append('\0');
                        if((rdData.count()>=reqLength)||(cnt==addr.count())) {
                            // отправить запрос

                            newRequest = true;
                            if(readOneRequest(port,reqAddr,rdData)) {
                                for(int i=0;i<rdData.length();++i) {
                                    result.append(std::make_pair(reqAddr+i,(unsigned char)rdData.at(i)));
                                }
                                curPercent+=cnt*stepPercent;
                                emit percentUpdate(curPercent);
                            }else break;
                        }
                    }else {
                        // отправить запрос

                        if(readOneRequest(port,reqAddr,rdData)) {
                            for(int i=0;i<rdData.length();++i) {
                                result.append(std::make_pair(reqAddr+i,(unsigned char)rdData.at(i)));
                            }
                            curPercent+=cnt*stepPercent;
                            emit percentUpdate(curPercent);
                        }else break;

                        reqAddr = ad;
                        rdData.clear();
                        rdData.append('\0');
                        if(cnt==addr.count()) {
                            // отправить запрос
                            if(readOneRequest(port,reqAddr,rdData)){
                                for(int i=0;i<rdData.length();++i) {
                                    result.append(std::make_pair(reqAddr+i,(unsigned char)rdData.at(i)));
                                }
                                curPercent+=cnt*stepPercent;
                                emit percentUpdate(curPercent);
                            }
                        }
                    }
                }
                prevAddr = ad;
            }
        }
        port.close();

    }else emit error("Ошибка открытия порта "+plc->getUartName());
    emit readFinished(result);
}

void sFram::stopProcess()
{
    QMutexLocker locker(&mutex);
    stopCmd = true;
}

