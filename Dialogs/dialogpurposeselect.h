#ifndef DIALOGPURPOSESELECT_H
#define DIALOGPURPOSESELECT_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "InputOutput/ioclassification.h"
#include "InputOutput/inputoutput.h"

namespace Ui {
class DialogPurposeSelect;
}

class DialogPurposeSelect : public QDialog
{
    Q_OBJECT
    QString purposeName;
    QString groupName;
    int code;
public:
    DialogPurposeSelect(IOClassification &ioclassification, InputOutput &io, QWidget *parent = 0);
    ~DialogPurposeSelect();
    QString getPurpose() const {return purposeName;}
    QString getGroupName() const {return groupName;}
    int getCode() const {return code;}
private:
    // строит дерево с данными типа ioType на основе ioclassification для узла parent
    // возвращает указатель на элемент дерева, код которого совпадает с code (или nullptr)
    QTreeWidgetItem* createPurposeTree(IOClassification &ioclassification, QTreeWidgetItem *parent, IOBase::IOTYPES ioType, int code);
private slots:

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::DialogPurposeSelect *ui;
};

#endif // DIALOGPURPOSESELECT_H
