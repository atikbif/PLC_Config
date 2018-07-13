#include "dialogpurposeselect.h"
#include "ui_dialogpurposeselect.h"

DialogPurposeSelect::DialogPurposeSelect(IOClassification &ioclassification,InputOutput &io, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPurposeSelect)
{
    ui->setupUi(this);
    QTreeWidgetItem *ioItem = nullptr;
    auto common = ioclassification.getCommonIOPurposes();
    for(const auto &purpose:common) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, purpose.first);
        item->setToolTip(0,QString::number(purpose.second));
        ui->treeWidget->addTopLevelItem(item);
        if(io.getCode()==purpose.second) ioItem = item;
    }
    if(io.getType()==IOBase::IOTYPES::DISCRETE_INPUT_OUTPUT) {
        QTreeWidgetItem *diNode = new QTreeWidgetItem();
        diNode->setText(0,"Дискретный вход");
        ui->treeWidget->addTopLevelItem(diNode);
        QTreeWidgetItem *res = createPurposeTree(ioclassification,diNode,IOBase::IOTYPES::DISCRETE_INPUT,io.getCode());
        if(ioItem==nullptr) ioItem = res;
        QTreeWidgetItem *doNode = new QTreeWidgetItem();
        doNode->setText(0,"Дискретный выход");
        ui->treeWidget->addTopLevelItem(doNode);
        res = createPurposeTree(ioclassification,doNode,IOBase::IOTYPES::DISCRETE_OUTPUT,io.getCode());
        if(ioItem==nullptr) ioItem = res;
    }else {
        QTreeWidgetItem *res = createPurposeTree(ioclassification,nullptr,io.getType(),io.getCode());
        if(ioItem==nullptr) ioItem = res;
    }

    if(ioItem) {
        purposeName = ioItem->text(0);
        code = ioItem->toolTip(0).toInt();
        QTreeWidgetItem *parent = ioItem->parent();
        if(parent) {
            parent->setExpanded(true);
            groupName = parent->text(0);
        }else groupName = "";
        ui->treeWidget->setCurrentItem(ioItem);
    }
}

DialogPurposeSelect::~DialogPurposeSelect()
{
    delete ui;
}

QTreeWidgetItem *DialogPurposeSelect::createPurposeTree(IOClassification &ioclassification, QTreeWidgetItem *parent, IOBase::IOTYPES ioType,int code)
{
    bool inputFlag = true;
    QTreeWidgetItem *res = nullptr;
    QStringList groups = ioclassification.getGroupNamesByType(ioType);
    for(QString group:groups) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,group);
        if(parent==nullptr) ui->treeWidget->addTopLevelItem(item);
        else parent->addChild(item);
        const auto &purposes = ioclassification.getIOPurposesByGroup(ioType,group);
        for(const auto &purpose:purposes) {
            inputFlag = true;
            if(ioclassification.isDO(purpose.second) || ioclassification.isAO(purpose.second)) inputFlag = false;
            QTreeWidgetItem *groupItem = new QTreeWidgetItem();
            groupItem->setText(0,purpose.first);
            groupItem->setToolTip(0,QString::number(purpose.second));
            if(inputFlag) groupItem->setTextColor(0,Qt::darkGreen);
            else groupItem->setTextColor(0,Qt::darkRed);

            item->addChild(groupItem);
            if(code==purpose.second) res = groupItem;
        }
    }
    return res;
}

void DialogPurposeSelect::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)
    if(current->childCount()) {
        purposeName = InputOutput::defaultIOState;
        code = 0;
        groupName = "";
    }
    else {
        purposeName = current->text(0);
        QTreeWidgetItem *parent = current->parent();
        if(parent) {
            groupName = parent->text(0);
        }else {
            groupName = "";
        }
        code = current->toolTip(0).toInt();
    }
}
