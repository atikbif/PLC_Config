#include "plcgui.h"
#include "ConfigHierarchy/jsondevicepatternreader.h"
#include "PLC/plcview.h"
#include "PLC/ioview.h"
#include "PLC/jsonplcviewreader.h"
#include "PLC/plcviewstorage.h"
#include "Dialogs/dialogpurposeselect.h"
#include "Dialogs/dialogdiconfig.h"
#include "Dialogs/dialogpressureconfig.h"
#include "Dialogs/dialogtemperatureconfig.h"
#include "Dialogs/dialoglevelconfig.h"
#include "Dialogs/dialogfrequencyconfig.h"
#include "Dialogs/dialogdefaultaiconfig.h"

#include <QDebug>

void PLCGUI::rectLeftClicked(PinGraphicsRectItem *item)
{
    int pinNum = item->getPinNumber();
    InputOutput io = plc.getInputOutputByPinNum(pinNum);

    std::unique_ptr<DialogPurposeSelect> dialog = std::make_unique<DialogPurposeSelect>(ioClassification,io);
    if(dialog->exec() == QDialog::Accepted) {
        QString purpose = dialog->getPurpose();
        QString groupName = dialog->getGroupName();
        int code = dialog->getCode();
        io.setGroupName(groupName);
        io.setCode(code);
        plc.updateInputOutput(io);
        QGraphicsTextItem* textItem = item->getTextItem();
        if(textItem) scene->removeItem(textItem);
        item->setTextItem(nullptr);

        if(purpose != InputOutput::defaultIOState) {
            QFont serifFont("Times", item->boundingRect().height()/2, QFont::Normal);
            serifFont.setItalic(true);
            bool textBeforeFlag = item->isTextBefore();

            QString purp = ioClassification.getGroupAndPurposeByCode(io.getCode()).second;
            textItem = new QGraphicsTextItem(purp,pSvgItem);
            textItem->setFont(serifFont);
            if(textBeforeFlag) {
                textItem->setPos(item->boundingRect().topLeft().x()-textItem->boundingRect().width()-10,item->boundingRect().topLeft().y()-3);
            }else {
                textItem->setPos(item->boundingRect().topRight().x()+10,item->boundingRect().topRight().y()-3);
            }

            textItem->setDefaultTextColor(Qt::black);
            if(ioClassification.isDI(code)) textItem->setDefaultTextColor(Qt::darkGreen);
            if(ioClassification.isDO(code)) textItem->setDefaultTextColor(Qt::darkRed);
            if(ioClassification.isAI(code)) textItem->setDefaultTextColor(Qt::darkGreen);
            if(ioClassification.isAO(code)) textItem->setDefaultTextColor(Qt::darkRed);
            textItem->setToolTip(QString::number(code));
            item->setTextItem(textItem);
        }
    }
}

void PLCGUI::rectRightClicked(PinGraphicsRectItem *item)
{
    int pinNum = item->getPinNumber();
    InputOutput io = plc.getInputOutputByPinNum(pinNum);
    int code = io.getCode();

    if(ioClassification.isAI(code)) {
        auto aiType = ioClassification.getAIType(code);
        switch(aiType) {
            case IOClassification::AI_TYPE::PRESSURE: {
                qDebug() << "Давление";
                auto dialog = std::make_unique<DialogPressureConfig>(AIConfig::State(io.getInputNumber(),aiType));
                QString title = "AI" + QString::number(io.getInputNumber()) + " (" + ioClassification.getGroupAndPurposeByCode(code).second + ")";
                dialog->setWindowTitle(title);
                if(dialog->exec()==QDialog::Accepted) {
                    AIConfig::State aiState = dialog->getAIState();
                    aiState.toMap();
                }
                }
                break;
            case IOClassification::AI_TYPE::TEMPERATURE: {
                qDebug() << "Температура";
                auto dialog = std::make_unique<DialogTemperatureConfig>(AIConfig::State(io.getInputNumber(),aiType));
                QString title = "AI" + QString::number(io.getInputNumber()) + " (" + ioClassification.getGroupAndPurposeByCode(code).second + ")";
                dialog->setWindowTitle(title);
                if(dialog->exec()==QDialog::Accepted) {
                    AIConfig::State aiState = dialog->getAIState();
                    aiState.toMap();
                }
                }
                break;
            case IOClassification::AI_TYPE::LEVEL: {
                qDebug() << "уровень";
                auto dialog = std::make_unique<DialogLevelConfig>(AIConfig::State(io.getInputNumber(),aiType));
                QString title = "AI" + QString::number(io.getInputNumber()) + " (" + ioClassification.getGroupAndPurposeByCode(code).second + ")";
                dialog->setWindowTitle(title);
                if(dialog->exec()==QDialog::Accepted) {
                    AIConfig::State aiState = dialog->getAIState();
                    aiState.toMap();
                }
                }
                break;
            case IOClassification::AI_TYPE::FREQUENCY: {
                qDebug() << "Обороты";
                auto dialog = std::make_unique<DialogFrequencyConfig>(AIConfig::State(io.getInputNumber(),aiType));
                QString title = "AI" + QString::number(io.getInputNumber()) + " (" + ioClassification.getGroupAndPurposeByCode(code).second + ")";
                dialog->setWindowTitle(title);
                if(dialog->exec()==QDialog::Accepted) {
                    AIConfig::State aiState = dialog->getAIState();
                    aiState.toMap();
                }
                }
                break;
            case IOClassification::AI_TYPE::UNKNOWN: {
                qDebug() << "неизвестный тип";
                auto dialog = std::make_unique<DialogDefaultAIConfig>(AIConfig::State(io.getInputNumber(),aiType));
                QString title = "AI" + QString::number(io.getInputNumber()) + " (" + ioClassification.getGroupAndPurposeByCode(code).second + ")";
                dialog->setWindowTitle(title);
                if(dialog->exec()==QDialog::Accepted) {
                    AIConfig::State aiState = dialog->getAIState();
                    aiState.toMap();
                }
                }
                break;
        }
    }else if(ioClassification.isDI(code)) {
        qDebug() << "Дискретный вход";
        auto dialog = std::make_unique<DialogDIConfig>(AIConfig::DISensor(io.getInputNumber()));
        QString title = "DI" + QString::number(io.getInputNumber()) + " (" + ioClassification.getGroupAndPurposeByCode(code).second + ")";
        dialog->setWindowTitle(title);
        if(dialog->exec()==QDialog::Accepted) {
            AIConfig::DISensor diState = dialog->getDIState();
            diState.toMap();
        }
    }
}

PLCGUI::PLCGUI(QGraphicsView *view, PLC &plc, IOClassification &ioClassification, QObject *parent):QObject(parent),
    view(view),plc(plc),ioClassification(ioClassification)
{
    pSvgItem = nullptr;
    scene = view->scene();
}

void PLCGUI::updatePLCGUI()
{
    auto ioTypes = plc.getInputOutputTypes();
    for(auto ioType:ioTypes) {
        const auto &ioList = plc.getInputOutputListByType(ioType);
        for(auto& io:ioList) {
            int pinNum = io.getPinNumber();
            if(rectItems.find(pinNum)!=rectItems.end()) {
                int code = io.getCode();
                auto item = rectItems[pinNum];
                QGraphicsTextItem* textItem = item->getTextItem();
                if(textItem) scene->removeItem(textItem);
                item->setTextItem(nullptr);

                if(code) {
                    QFont serifFont("Times", item->boundingRect().height()/2, QFont::Normal);
                    serifFont.setItalic(true);
                    bool textBeforeFlag = item->isTextBefore();
                    QString purp = ioClassification.getGroupAndPurposeByCode(io.getCode()).second;
                    textItem = new QGraphicsTextItem(purp,pSvgItem);
                    textItem->setFont(serifFont);
                    if(textBeforeFlag) {
                        textItem->setPos(item->boundingRect().topLeft().x()-textItem->boundingRect().width()-10,item->boundingRect().topLeft().y()-3);
                    }else {
                        textItem->setPos(item->boundingRect().topRight().x()+10,item->boundingRect().topRight().y()-3);
                    }

                    textItem->setDefaultTextColor(Qt::black);
                    if(ioClassification.isDI(code)) textItem->setDefaultTextColor(Qt::darkGreen);
                    if(ioClassification.isDO(code)) textItem->setDefaultTextColor(Qt::darkRed);
                    if(ioClassification.isAI(code)) textItem->setDefaultTextColor(Qt::darkGreen);
                    if(ioClassification.isAO(code)) textItem->setDefaultTextColor(Qt::darkRed);
                    textItem->setToolTip(QString::number(code));
                    item->setTextItem(textItem);
                }
            }
        }
    }
}

StandardConfigStorage::Config PLCGUI::newPLC(const QString &fName)
{
    scene->clear();

    std::unique_ptr<DevicePatternReader> deviceReader = std::make_unique<JSONDevicePatternReader>();
    StandardConfigStorage::Config confInfo = deviceReader->getConfigInfo(fName);

    QString plcName = confInfo.plc;

    std::unique_ptr<IPLCViewReader> plcReader = std::make_unique<JSONPLCViewReader>();
    PLCViewStorage storage(std::move(plcReader),":/JSON/plc.json");

    PLCView plcView = storage.getPLCViewByName(plcName);
    QString image = plcView.getImageFileName();

    pSvgItem = new QGraphicsSvgItem(image);

    std::size_t offset_x = width/2 - pSvgItem->boundingRect().width()/2;
    std::size_t offset_y = height/100;

    pSvgItem->setPos(offset_x,offset_y);
    pSvgItem->setFlags(QGraphicsItem::ItemIsMovable);
    rectItems.clear();

    scene->addItem(pSvgItem);

    plc = plcView.getEmptyPLC();
    QString color = plcView.getColor();
    double opacity = plcView.getOpacity();
    QVector<IOView> ioViews = plcView.getIOViews();
    for(const auto &ioView:ioViews) {
        auto pinRect = new PinGraphicsRectItem(ioView.getX(), ioView.getY(), ioView.getWidth(), ioView.getHeight(), pSvgItem);
        pinRect->setPinNumber(ioView.getPinNumber());
        pinRect->setBrush(QBrush(QColor(color)));
        pinRect->setOpacity(opacity);
        pinRect->setTextBeforeFlag(ioView.isTextBefore());
        connect(pinRect,SIGNAL(leftClick(PinGraphicsRectItem*)),this,SLOT(rectLeftClicked(PinGraphicsRectItem*)));
        connect(pinRect,SIGNAL(rightClick(PinGraphicsRectItem*)),this,SLOT(rectRightClicked(PinGraphicsRectItem*)));
        rectItems[pinRect->getPinNumber()] = pinRect;
    }
    view->scale(resetScale,resetScale);
    resetScale = 1;

    plc.setIOState(deviceReader->getDevicePattern(fName),ioClassification);
    updatePLCGUI();
    return confInfo;
}

void PLCGUI::zoom_in()
{
    view->scale(1.25,1.25);
    resetScale*=0.8;
}

void PLCGUI::zoom_out()
{
    view->scale(0.8,0.8);
    resetScale*=1.25;
}

