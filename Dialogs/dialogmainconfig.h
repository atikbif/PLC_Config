#ifndef DIALOGMAINCONFIG_H
#define DIALOGMAINCONFIG_H

#include <QDialog>

namespace Ui {
class DialogMainConfig;
}

class DialogMainConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMainConfig(bool pultState, QWidget *parent = 0);
    bool getPultState();
    ~DialogMainConfig();

private:
    Ui::DialogMainConfig *ui;
};

#endif // DIALOGMAINCONFIG_H
