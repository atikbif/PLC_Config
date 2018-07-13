#include <memory>
#include <QIcon>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QFileInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "InputOutput/jsonioclassificationreader.h"
#include "InputOutput/aiconfig.h"
#include "ConfigHierarchy/jsonstandardconfigreader.h"
#include "Dialogs/dialognewproject.h"
#include "Dialogs/dialogdeviceselect.h"
#include "Dialogs/dialogabout.h"
#include "Dialogs/dialogmainconfig.h"
#include "AutoSearch/scangui.h"
#include "Loader/bootmodesetter.h"
#include "Loader/ymodemthread.h"
#include "Loader/sysframreadwrite.h"
#include "FRAM/boilpinframconverter.h"


/*#include "boost/date_time/gregorian/gregorian.hpp"
using namespace boost::gregorian;*/


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::unique_ptr<IIOClassificationReader> ioReader = std::make_unique<JSONIOClassificationReader>();
    ioReader->readIOClassification(":/JSON/io.json",ioClassification);
    createToolBar();
    disableActions();

    scene = new QGraphicsScene();
    scene->setSceneRect(QRectF(0, 0, PLCGUI::width, PLCGUI::height));
    view = new QGraphicsView(scene);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    ui->widget->setLayout(layout);
    showMaximized();
    path = "/";
    gui = std::make_unique<PLCGUI>(view,plc,ioClassification);

    /*date today = day_clock::local_day();
    partial_date new_years_day(1,Jan);
    days days_since_year_start = today - new_years_day.get_date(today.year());
    setWindowTitle(QString::number(days_since_year_start.days()) + "\n");*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openPattern(const QString &fName)
{
    confInfo = gui->newPLC(fName);
    QString info = confInfo.baseType + ": " + confInfo.baseDevice + ": " + confInfo.name;
    setWindowTitle(info);
}

void MainWindow::enableActions()
{
    saveProjectAction->setEnabled(true);
    saveAsAction->setEnabled(true);
    zoomInAction->setEnabled(true);
    zoomOutAction->setEnabled(true);
    //debugAction->setEnabled(true);
    confAction->setEnabled(true);
    writeConfigAction->setEnabled(true);
    readConfigAction->setEnabled(true);
    printAction->setEnabled(true);
}

void MainWindow::disableActions()
{
    saveProjectAction->setEnabled(false);
    saveAsAction->setEnabled(false);
    zoomInAction->setEnabled(false);
    zoomOutAction->setEnabled(false);
    debugAction->setEnabled(false);
    writeConfigAction->setEnabled(false);
    readConfigAction->setEnabled(false);
    printAction->setEnabled(false);
    confAction->setEnabled(false);
}

void MainWindow::createToolBar()
{
    newProjectAction = ui->mainToolBar->addAction(QIcon(":/Icons/new_32.ico"),"Новый проект (Ctrl+N)",this,&MainWindow::new_project);
    openProjectAction = ui->mainToolBar->addAction(QIcon(":/Icons/open.ico"),"Открыть проект (Ctrl+O)",this,&MainWindow::open_project);
    saveProjectAction = ui->mainToolBar->addAction(QIcon(":/Icons/save.ico"),"Сохранить проект (Ctrl+S)",this,&MainWindow::save_project);
    saveAsAction = ui->mainToolBar->addAction(QIcon(":/Icons/save_as.ico"),"Сохранить как",this,&MainWindow::save_as);
    zoomInAction = ui->mainToolBar->addAction(QIcon(":/Icons/zoom_in.ico"),"Увеличить масштаб (Ctrl+Plus)",this,&MainWindow::zoom_in);
    zoomOutAction = ui->mainToolBar->addAction(QIcon(":/Icons/zoom_out.ico"),"Уменьшить масштаб (Ctrl+Minus)",this,&MainWindow::zoom_out);
    confAction = ui->mainToolBar->addAction(QIcon(":/Icons/settings.ico"),"Настройки",this,&MainWindow::config);
    debugAction = ui->mainToolBar->addAction(QIcon(":/Icons/debug.ico"),"Отладка",this,&MainWindow::connect_to_plc);
    writeFlashAction = ui->mainToolBar->addAction(QIcon(":/Icons/write_flash.ico"),"Загрузить программу (F7)",this,&MainWindow::write_flash);
    writeConfigAction = ui->mainToolBar->addAction(QIcon(":/Icons/write_config.ico"),"Загрузить конфигурацию (F8)",this,&MainWindow::write_config);
    readConfigAction = ui->mainToolBar->addAction(QIcon(":/Icons/read_config.ico"),"Прочитать конфигурациию (F9)",this,&MainWindow::read_config);
    printAction = ui->mainToolBar->addAction(QIcon(":/Icons/print.ico"),"Печать (Ctrl+P)",this,&MainWindow::print);
    ui->mainToolBar->addAction(QIcon(":/Icons/about.ico"),"О программе",this,&MainWindow::about);
}

void MainWindow::new_project()
{
    std::unique_ptr<DialogNewProject> dialog = std::make_unique<DialogNewProject>();
    std::unique_ptr<StandardConfigReader> confReader = std::make_unique<JSONStandardConfigReader>() ;
    StandardConfigStorage confStorage = confReader->readStandardConfig(":/JSON/devices.json");
    if(dialog->exec() == QDialog::Accepted) {
        if(dialog->getType()==DialogNewProject::DeviceType::BOIL) {
            std::unique_ptr<DialogDeviceSelect> dialogDevice = std::make_unique<DialogDeviceSelect>(confStorage,1);
            if(dialogDevice->exec() == QDialog::Accepted) {
                QString confFile = dialogDevice->getConfFile();
                openPattern(confFile);
                enableActions();
                saveProjectAction->setEnabled(false);

                AIConfig::readEEMap(":/JSON/" + confInfo.defaultEE);
            }
        }
        pult_state = true;
    }
}

void MainWindow::open_project()
{
    QString fName = QFileDialog::getOpenFileName(this, tr("Открытие проекта"),
        path,
        tr("Конфигурация ПЛК (*.plc )"));
    if(!fName.isEmpty()) {
        QFileInfo fInfo(fName);
        path = fInfo.absolutePath();
        openPattern(fName);
        projectPatternFileName = fName;
        enableActions();

        QFileInfo fileInfo(projectPatternFileName);
        QString eeShortFileName = fileInfo.fileName();
        eeShortFileName.remove(QRegExp("\\.plc$"));
        eeShortFileName += ".par";
        confInfo.defaultEE = eeShortFileName;
        QString eeFullFileName = projectPatternFileName;
        eeFullFileName.remove(QRegExp("\\.plc$"));
        eeFullFileName += ".par";
        AIConfig::readEEMap(eeFullFileName);
        pult_state = true;
    }
}

void MainWindow::save_project()
{

    QFile saveFile(projectPatternFileName);

    if (saveFile.open(QIODevice::WriteOnly)) {
        QFileInfo fileInfo(projectPatternFileName);
        QString eeShortFileName = fileInfo.fileName();
        eeShortFileName.remove(QRegExp("\\.plc$"));
        eeShortFileName += ".par";
        confInfo.defaultEE = eeShortFileName;
        QString eeFullFileName = projectPatternFileName;
        eeFullFileName.remove(QRegExp("\\.plc$"));
        eeFullFileName += ".par";
        AIConfig::writeEEMap(eeFullFileName);

        QJsonObject saveObject;
        saveObject["version"] = 1;
        saveObject["type"] = confInfo.baseType;
        saveObject["device"] = confInfo.baseDevice;
        saveObject["config"] = confInfo.name;
        saveObject["plc"] = confInfo.plc;
        saveObject["comment"] = confInfo.comment;
        saveObject["EEFile"] = confInfo.defaultEE;
        QJsonArray pinList;
        auto ioTypes = plc.getInputOutputTypes();
        for(auto ioType:ioTypes) {
            const auto &ioList = plc.getInputOutputListByType(ioType);
            for(auto& io:ioList) {
                int pinNum = io.getPinNumber();
                int code = io.getCode();
                QJsonObject pinObject;
                pinObject["pin"] = pinNum;
                pinObject["code"] = code;
                pinList.append(pinObject);
            }
        }
        saveObject["pins"] = pinList;
        QJsonDocument saveDoc(saveObject);
        saveFile.write(saveDoc.toJson());

        ui->statusBar->showMessage("Проект сохранён",2000);
    }else {
        QString message = "Couldn't open file";
        qWarning(message.toStdString().c_str());
        ui->statusBar->showMessage("Ошибка сохранения проекта",2000);
    }
}

void MainWindow::save_as()
{
    QString fName = QFileDialog::getSaveFileName(this, tr("Сохранение проекта"),
                                                        path,
                                                        tr("Конфигурация ПЛК (*.plc )"));
    QFileInfo fInfo(fName);
    path = fInfo.absolutePath();
    projectPatternFileName = fName;

    save_project();
    saveProjectAction->setEnabled(true);
}

void MainWindow::zoom_in()
{
    gui->zoom_in();
}

void MainWindow::zoom_out()
{
    gui->zoom_out();
}

void MainWindow::connect_to_plc()
{

}

void MainWindow::write_flash()
{
    ScanGUI gui(0,true,"AUTO",this);
    int ret = gui.exec();
    if(ret==QDialog::Accepted) {
        BootModeSetter bootSetter(this);
        if(bootSetter.setBootMode()) {
            DetectedController* plc = &DetectedController::Instance();
            YmodemThread loader(plc->getUartName(),this);
            loader.exec();
        }
    }
}

void MainWindow::write_config()
{
    ScanGUI gui(0,false,"AUTO",this);
    int ret = gui.exec();
    if(ret==QDialog::Accepted) {
        DetectedController* detectedPLC = &DetectedController::Instance();
        if(detectedPLC->getBootMode()) QMessageBox::warning(this,
                "настройки контроллера",
                "Контроллер ожидает загрузки программы.\nЧтение/запись настроек невозможны.");
        else{
            SysFramReadWrite loader(this);
            std::unique_ptr<PinFRAMConverter> converter = std::make_unique<BoilPinFRAMConverter>();
            QVector<std::pair<int,int>> data = converter->pinToFram(ioClassification,plc);
            auto pultData = pult_state?std::make_pair(0x7F57,0):std::make_pair(0x7F57,0x31);
            data.append(pultData);

            connect(this,SIGNAL(writeConf(QVector<std::pair<int,int>>)),&loader,
                    SLOT(startWriteProcess(QVector<std::pair<int,int>>)));
            emit writeConf(data);
            loader.exec();
            disconnect(this,SIGNAL(writeConf(QVector<std::pair<int,int>>)),&loader,
                       SLOT(startWriteProcess(QVector<std::pair<int,int>>)));
        }
    }
}

void MainWindow::read_config()
{
    ScanGUI gui(0,false,"AUTO",this);
    int ret = gui.exec();
    if(ret==QDialog::Accepted) {
        DetectedController* detectedPLC = &DetectedController::Instance();
        if(detectedPLC->getBootMode()) QMessageBox::warning(this,
                "настройки контроллера",
                "Контроллер ожидает загрузки программы.\nЧтение/запись настроек невозможны.");
        else{
            SysFramReadWrite loader(this);
            std::unique_ptr<PinFRAMConverter> converter = std::make_unique<BoilPinFRAMConverter>();
            QVector<int> addrList = converter->getFramAddressList();

            for(auto& pair:AIConfig::eeBufMap) {
                addrList.append(BoilPinFRAMConverter::startOffset+pair.first);
                addrList.append(BoilPinFRAMConverter::startOffset+pair.first+1);
            }
            addrList.append(0x7F57);

            connect(this,SIGNAL(readConf(QVector<int>)),&loader,SLOT(startReadProcess(QVector<int>)));
            connect(&loader,SIGNAL(readOK(QVector<std::pair<int,int> >)),
                    this,SLOT(readConfResult(QVector<std::pair<int,int> >)));
            emit readConf(addrList);
            loader.exec();
            disconnect(this,SIGNAL(readConf(QVector<int>)),&loader,SLOT(startReadProcess(QVector<int>)));
            ui->statusBar->showMessage("Конфигурация прочитана",2000);
        }
    }
}

void MainWindow::print()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOrientation(QPrinter::Landscape);
    QPrintDialog dialog(&printer, this);
    if(dialog.exec()==QPrintDialog::Accepted) {
        QPainter painter(&printer);
        view->render(&painter);
    }
}

void MainWindow::about()
{
    DialogAbout dialog;
    dialog.exec();
}

void MainWindow::readConfResult(QVector<std::pair<int, int> > res)
{
    std::unique_ptr<PinFRAMConverter> converter = std::make_unique<BoilPinFRAMConverter>();
    converter->framToPLC(res,ioClassification,plc);
    auto pult_conf_it = std::find_if(res.begin(),res.end(),[](const auto& pair){return pair.first==0x7F57;});
    if(pult_conf_it!=res.end()) {
        int value = pult_conf_it->second;
        if(value==0x31) pult_state=false;else pult_state=true;
    }
    gui->updatePLCGUI();
}

void MainWindow::config()
{
    auto dialog = std::make_unique<DialogMainConfig>(pult_state);
    if(dialog->exec() == QDialog::Accepted) {
        pult_state = dialog->getPultState();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_Plus) {if(zoomInAction->isEnabled()) zoom_in();}
        if (event->key() == Qt::Key_Minus) {if(zoomOutAction->isEnabled()) zoom_out();}
        if (event->key() == Qt::Key_N) {if(newProjectAction->isEnabled()) new_project();}
        if (event->key() == Qt::Key_S) {
            if(saveProjectAction->isEnabled()) save_project();
            else if(saveAsAction->isEnabled()) save_as();
        }
        if (event->key() == Qt::Key_O ) {if(openProjectAction->isEnabled()) open_project();}
        if (event->key() == Qt::Key_P) {if(printAction->isEnabled()) print();}
    }

    if (event->key() == Qt::Key_F7) {if(writeFlashAction->isEnabled()) write_flash();}
    if (event->key() == Qt::Key_F8) {if(writeConfigAction->isEnabled()) write_config();}
    if (event->key() == Qt::Key_F9) {if(readConfigAction->isEnabled()) read_config();}
    QMainWindow::keyPressEvent(event);
}
