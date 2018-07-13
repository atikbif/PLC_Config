#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <memory>
#include <utility>

#include "InputOutput/ioclassification.h"
#include "PLC/plc.h"
#include "PLCGUI/plcgui.h"
#include "ConfigHierarchy/standardconfigstorage.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QGraphicsView *view;
    QGraphicsScene *scene;

    IOClassification ioClassification;
    PLC plc;
    StandardConfigStorage::Config confInfo;

    QAction *newProjectAction;
    QAction *openProjectAction;
    QAction *saveProjectAction;
    QAction *saveAsAction;
    QAction *debugAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *writeFlashAction;
    QAction *writeConfigAction;
    QAction *readConfigAction;
    QAction *printAction;
    QAction *confAction;


    std::unique_ptr<PLCGUI> gui;

    QString projectPatternFileName;
    QString path;

    bool pult_state = true;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void openPattern(const QString &fName);
    void enableActions();
    void disableActions();
    void createToolBar();

private slots:
    void new_project();
    void open_project();
    void save_project();
    void save_as();
    void zoom_in();
    void zoom_out();
    void connect_to_plc();
    void write_flash();
    void write_config();
    void read_config();
    void print();
    void about();
    void readConfResult(QVector<std::pair<int,int>> res);
    void config();

signals:
    void writeConf(QVector<std::pair<int,int>>); // адрес ячейки/значение
    void readConf(QVector<int>); // адреса ячеек для чтения

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
