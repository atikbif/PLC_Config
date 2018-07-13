#include "jsonplcviewreader.h"
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>

const std::map<QString,IOBase::IOTYPES> JSONPLCViewReader::stringTypes = {
    {"di", IOBase::IOTYPES::DISCRETE_INPUT},
    {"do", IOBase::IOTYPES::DISCRETE_OUTPUT},
    {"dio", IOBase::IOTYPES::DISCRETE_INPUT_OUTPUT},
    {"ai", IOBase::IOTYPES::ANALOG_INPUT},
    {"ao", IOBase::IOTYPES::ANALOG_OUTPUT}
};

QVector<PLCView> JSONPLCViewReader::read(const QJsonObject &json)
{
    QVector<PLCView> plcViews;
    int version = json["version"].toInt();
    Q_UNUSED(version)
    QJsonArray plcList = json["plc"].toArray();
    for(const auto &plc:plcList) {
        QJsonObject plcObject = plc.toObject();
        QString plcName = plcObject["name"].toString();
        QString imageFileName = plcObject["image"].toString();
        double opacity = plcObject["opacity"].toDouble();
        QString color = plcObject["color"].toString();
        auto ioList = plcObject["io"].toArray();
        PLCView plcView;
        plcView.setName(plcName);
        plcView.setImageFileName(imageFileName);
        plcView.setColor(color);
        plcView.setOpacity(opacity);
        for(const auto &io:ioList) {
            addIOView(plcView,io);
        }
        plcViews.append(plcView);
    }
    return plcViews;
}

void JSONPLCViewReader::addIOView(PLCView &plcView, const QJsonValue &io)
{
    QJsonObject ioObject = io.toObject();
    int x = ioObject["x"].toInt();
    int y = ioObject["y"].toInt();
    int width = ioObject["width"].toInt();
    int height = ioObject["height"].toInt();
    IOBase::IOTYPES type = IOBase::IOTYPES::DISCRETE_INPUT;
    QString sType = ioObject["type"].toString();
    auto it = stringTypes.find(sType);
    if(it!=stringTypes.end()) {
        type = it->second;
    }
    int pinNumber = ioObject["pinNumber"].toInt();
    int inpNumber = ioObject["inpNumber"].toInt();
    int outNumber = ioObject["outNumber"].toInt();
    bool textBeforeFlag = ioObject["text before"].toBool();
    IOView ioView;
    ioView.setX(x);
    ioView.setY(y);
    ioView.setWidth(width);
    ioView.setHeight(height);
    ioView.setType(type);
    ioView.setPinNumber(pinNumber);
    ioView.setInputNumber(inpNumber);
    ioView.setOutputNumber(outNumber);
    ioView.setTextBeforeFlag(textBeforeFlag);
    plcView.addIOView(ioView);
}

QVector<PLCView> JSONPLCViewReader::readData(const QString &fName)
{
    QFile loadFile(fName);

    if (loadFile.open(QIODevice::ReadOnly)) {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        return read(loadDoc.object());
    }else {
        QString message = "Couldn't open file " + fName;
        qWarning(message.toStdString().c_str());
    }
    return QVector<PLCView>();
}
