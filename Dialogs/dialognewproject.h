#ifndef DIALOGNEWPROJECT_H
#define DIALOGNEWPROJECT_H

#include <QDialog>

namespace Ui {
class DialogNewProject;
}

class DialogNewProject : public QDialog
{
    Q_OBJECT
public:
    enum class DeviceType {BOIL,VENT};
private:
    DeviceType dev;

public:
    explicit DialogNewProject(QWidget *parent = 0);
    ~DialogNewProject();
    DeviceType getType() const {return dev;}

private slots:
    void on_pushButtonBoil_clicked();

    void on_pushButtonVent_clicked();

private:
    Ui::DialogNewProject *ui;
};

#endif // DIALOGNEWPROJECT_H
