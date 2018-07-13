#ifndef DIALOGDEVICESELECT_H
#define DIALOGDEVICESELECT_H

#include <QDialog>
#include "ConfigHierarchy/standardconfigstorage.h"
#include <QListWidgetItem>

namespace Ui {
class DialogDeviceSelect;
}

class DialogDeviceSelect : public QDialog
{
    Q_OBJECT
    QVector<StandardConfigStorage::Device> confDevices;
    QVector<StandardConfigStorage::Config> currentConfigs;
    QString confFile;
    QString plc;
    QString comment;
public:
    explicit DialogDeviceSelect(const StandardConfigStorage &storage, int typeNum, QWidget *parent = 0);
    ~DialogDeviceSelect();
    QString getConfFile() const {return confFile;}
    QString getPLCName() const {return plc;}
    QString getComment() const {return comment;}

private slots:
    void on_listWidgetDevices_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidgetConfigs_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::DialogDeviceSelect *ui;
};

#endif // DIALOGDEVICESELECT_H
