#include "pusettlewin.h"
#include "ui_pusettlewin.h"
#include "haokeapp.h"
#include "bean/DepBean.h"
#include "bean/SupplieBean.h"
#include "bean/EmployeeBean.h"
#include "bean/WareHouseBean.h"
#include "bean/ProductBean.h"
#include "bean/SystemSetBean.h"
#include "dao/depdao.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "dao/employeedao.h"
#include "dao/waredao.h"
#include "dao/productdao.h"
#include "dao/pusettledao.h"
#include "dao/systemsetdao.h"
#include "dao/rddao.h"
#include "utils/puitemutil.h"
#include "common/cachebabeancommon.h"
#include "common/printutils.h"
#include "qtrpt.h"
#include "bean/ReportFormatBean.h"
#include "dao/reportdao.h"
#include "widget/designoptdialog.h"
#include "widget/choosevouchdialog.h"
#include "widget/optinvdialog.h"
#include "cgplugin.h"

PuSettleWin::PuSettleWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PuSettleWin)
{
    ui->setupUi(this);
    init();
    //qApp->setStyle("Plastique");
    setupContextMenu();
    mCountRecord = PuSettleDAO::getPuSettleCount("01") - 1;
    mRecord = mCountRecord;
    if(mCountRecord < 0) mCountRecord = 0;
    QList<PuSettleVouchBean *> vouchBeanList = PuSettleDAO::queryPuSettleByPage(mCountRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(1)->setEnabled(false);
    baseToolBar->actions().at(3)->setEnabled(false);
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(7)->setEnabled(true);
    if(ui->noEdit->text().length() >0){
        ui->toolBar->actions().at(2)->setEnabled(true);
    }
    readAuthority();
}

PuSettleWin::PuSettleWin(QString no,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PuSettleWin)
{
    ui->setupUi(this);
    init();
    setupContextMenu();
    mCountRecord = PuSettleDAO::getPuSettleCount("01") - 1;
    if(mCountRecord < 0) mCountRecord = 0;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(7)->setEnabled(true);

    PuSettleVouchBean *bean = PuSettleDAO::getPuSettleByNo(no);
    if(bean != NULL && bean->no().length() > 0){
        mRecord = PuSettleDAO::getPuSettleRowNumber(bean->id()) - 1;
        if(mRecord == 0){
            baseToolBar->actions().at(0)->setEnabled(false);
            baseToolBar->actions().at(2)->setEnabled(false);
        }else if(mRecord >= mCountRecord ){
            baseToolBar->actions().at(1)->setEnabled(false);
            baseToolBar->actions().at(3)->setEnabled(false);
        }
        delete bean;
        read(no);
        if(ui->noEdit->text().length() >0){
            ui->toolBar->actions().at(2)->setEnabled(true);
        }
    }else{
        QMessageBox::critical(this,"错误","此单不存在！");
    }
    readAuthority();
}

PuSettleWin::~PuSettleWin()
{
    delete ui;
}

void PuSettleWin::readAuthority(){
    LimitsCommon::authority(ui->toolBar,mWinId.toInt());
}

void PuSettleWin::init(){

    //窗体ID号
    mWinId = HaoKeApp::puSubId() + CgPlugin::settleWinId();
    mWinTitle = "采购结算单";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::puSubId().toInt(),mWinId.toInt(),0,"");

    //功能菜单
    actionProperties actsBase[]= {
        { "puSettle.new",tr("&New"),SLOT(sNew()), NULL, "New", QPixmap(":/image/Add.png"), NULL,  true, tr("New") },
        { "puSettle.edit",tr("&Edit"),SLOT(sEdit()), NULL, "Edit", QPixmap(":/image/edit.png"), NULL,  true, tr("Edit") },
        { "puSettle.delete",tr("&Delete"),SLOT(sDelete()), NULL, "Delete", QPixmap(":/image/cross_48.png"), NULL,  true, tr("Delete") },
        { "puSettle.save",tr("&Save"),SLOT(sSave()), NULL, "Save", QPixmap(":/image/save.png"), NULL,  true, tr("Save") },
        { "puSettle.audit",tr("&Audit"),SLOT(sAudit()), NULL, "Audit", QPixmap(":/image/stamp_48.png"), NULL,  true, tr("Audit") },
        { "puSettle.abandonAudit",tr("&Abandon Audit"),SLOT(sAbAudit()), NULL, "Abandon Audit", QPixmap(":/image/c_stamp_48.png"), NULL,  true, tr("Abandon Audit") },
        { "puSettle.delRow",tr("&DelRow"),SLOT(sDelRow()), NULL, "DelRow", QPixmap(":/image/minus.png"), NULL,  true, tr("DelRow") },
        { "puSettle.print",tr("&Print"),SLOT(sPrint()), NULL, "Print", QPixmap(":/image/printer.png"), NULL,  true, tr("Print") },
        { "puSettle.choose",tr("&Choose"),SLOT(sChoose()), NULL, "Choose", QPixmap(":/image/page_accept.png"), NULL,  true, tr("Choose") },
        { "puSettle.first",tr("&First"),SLOT(sFirst()), NULL, "First", QPixmap(":/image/first48.png"), NULL,  true, tr("First") },
        { "puSettle.next",tr("&Next"),SLOT(sNext()), NULL, "Next", QPixmap(":/image/n-next48.png"), NULL,  true, tr("Next") },
        { "puSettle.previous",tr("&Previous"),SLOT(sPrevious()), NULL, "Previous", QPixmap(":/image/n-previous48.png"), NULL,  true, tr("Previous") },
        { "puSettle.end",tr("&End"),SLOT(sEnd()), NULL, "End", QPixmap(":/image/end48.png"), NULL,  true, tr("End") },
        { "puSettle.cancel",tr("&Cancel"),SLOT(sCancel()), NULL, "Cancel", QPixmap(":/image/cancel.png"), NULL,  true, tr("Cancel") }

    };
    addActionsTo(actsBase,sizeof(actsBase) / sizeof(actsBase[0]));

    mIsEdit =false;

    ui->dateEdit->setDate(HaoKeApp::getInstance()->getOperateDate());
    mIsChoose = false;

    //初始化表体格式
    setInputEnable(false);

    childItemProperties childItem[] = {
        { "productCode","商品编码","商品编码", 0,180, true, true },
        { "productName","商品名称","商品名称", 1,180, true, true },
        { "productStyle","商品规格","商品规格", 2,80, true, false },
        { "unit","单位","单位", 3,80, true, false },
        { "quantity","结算数量","结算数量", 4, 60,true, true },
        { "price","原单价","原单价", 5,80, true, true },
        { "sPrice","结算单价","结算单价", 6,80, true, true },
        { "amount","结算金额","结算金额", 7,80, true, false },
        { "rdNo","入库单号","入库单号", 8,100, true, false },
        { "id","ID","ID", 9,60, false, false },



    };
    int size = sizeof(childItem) / sizeof(childItem[0]);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(size);
    //qDebug() << size;
    for(int i =0 ; i < size ; i ++){
        mChildItem << childItem[i];
    }
     //初始化表尾
    mFootView = new HierarchicalHeaderView(Qt::Horizontal);
    mFootView->setClickable(true);
    //mFootView->setMovable(false);
    mFootView->setResizeMode(QHeaderView::Fixed);
    mFootView->setHighlightSections(true);
    mFootModel = new FootTableModel(1,mChildItem.size(),this);
    mFootModel->setItem(0,new QStandardItem(""));
    mFootModel->setItem(1,new QStandardItem("合计:"));
    mFootView->setModel(mFootModel);

    mFootView->resizeSection(0,ui->tableWidget->verticalHeader()->width());
    mFootView->setFixedHeight(22);

    ui->verticalLayout_2->addWidget(mFootView);
    mFootModel->setItem(size - 1,new QStandardItem(""));
    QObject::connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(footResized(int,int,int)));
    refreshData();
    /*
    ui->tableWidget->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    */
    mFootView->setDefaultAlignment (Qt::AlignLeft | Qt::AlignVCenter);
    mFootView->setTextElideMode (Qt::ElideLeft); //...效果


    //初始化表头数据
    QStringList  busTypeList;
    busTypeList << "普通结算";
    busTypeList << "红字结算";
    ui->busTypeComboBox->addItems(busTypeList);
    QList<SupplieBean *> venBeanList = SupplieDAO::querySupplie();
    QStringList venList ;
    for(int i =0 ; i < venBeanList.size(); i++){
        venList << venBeanList.at(i)->supplieCode() + " - " +venBeanList.at(i)->supplieName();
    }
    ui->venSearchEditbox->setListModel(true);
    ui->venSearchEditbox->setItems(venList);

    QList<DepBean *> depBeanList = DepDAO::queryDepByEnd();
    QStringList depList ;
    for(int i =0 ; i < depBeanList.size(); i++){
        depList << depBeanList.at(i)->depId() + " - " +depBeanList.at(i)->depName();
    }
    ui->depSearchEditbox->setListModel(true);
    ui->depSearchEditbox->setItems(depList);

    QList<EmployeeBean *> employeeBeanList = EmployeeDAO::queryEmp();
    QStringList empList ;
    for(int i =0 ; i < employeeBeanList.size(); i++){
        empList << employeeBeanList.at(i)->empCode() + " - " +employeeBeanList.at(i)->empName();
    }
    ui->userSearchEditbox->setListModel(true);
    ui->userSearchEditbox->setItems(empList);

    QList<ProductBean *> proBeanList = ProductDAO::queryProduct();
    QStandardItemModel *model = new QStandardItemModel(proBeanList.size(),1);
    for(int i=0;i < proBeanList.size();i++){
        QStandardItem *itemName = new QStandardItem(proBeanList.at(i)->productCode() + " - " +proBeanList.at(i)->productName());
        model->setItem(i,0,itemName);
    }
    QkEditDelegate *delegate = new QkEditDelegate(model,this);
    connect(delegate,SIGNAL(btnClicked()),this,SLOT(sInvClick()));
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(22);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setItemDelegateForColumn(0,delegate);

}

void PuSettleWin::sInvClick(){
    QString sql = " SELECT a.* FROM ba_product a"
            " WHERE a.product_code LIKE ? AND a.type_id LIKE ? ";
    childItemProperties childItem[] = {
            { "product_code","编码商品","商品", 0,60, true, true },
            { "product_name","名称商品","商品", 1,100, true, true },
            { "style","规格","规格", 2,80, true, true },
            { "purchase_unit","单位","单位", 3, 60,true, true },
            { "pu_price","价格","价格", 4, 60,true, true }
    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    QString invCode = "";
    OptInvDialog *invDlg = new OptInvDialog(dItem,sql,invCode,this);

    if(invDlg->exec() == QDialog::Accepted){
        QItemSelectionModel *selectionModel = invDlg->getTableView()->selectionModel();
        QModelIndexList selected= selectionModel->selectedIndexes();
        if(selected.size() < 1){
            QMessageBox::information(this,"提示","末选择商品!");

        }else{
            invCode = invDlg->getTableView()->model()->data(invDlg->getTableView()->model()->index(selected.at(0).row(),0)).toString();
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,ui->tableWidget->currentRow(),mChildItem,"productCode",invCode);
        }
    }
    delete invDlg;
}
void PuSettleWin::footResized(int logicalIndex, int oldSize, int newSize){
    mFootView->resizeSection(logicalIndex + 1,newSize);
}

void PuSettleWin::refreshData(){

    QStringList headerLabels;
    for (int i = 0; i < mChildItem.size(); i++){
        headerLabels.append(mChildItem[i].cnName);
        ui->tableWidget->setColumnWidth(mChildItem[i].id,mChildItem[i].width);
        mFootView->resizeSection(mChildItem[i].id + 1,mChildItem[i].width);
        if(!mChildItem[i].visible)ui->tableWidget->hideColumn(mChildItem[i].id);
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
}

void PuSettleWin::read(QString no){
    PuSettleVouchBean *bean = PuSettleDAO::getPuSettleByNo(no);
    mNo = no;
    if(bean != NULL && bean->no().length() > 0){
        ui->noEdit->setText(bean->no());
        ui->dateEdit->setDate(bean->settleDate());
        SupplieBean *venBean = SupplieDAO::getSupplieByCode(bean->supplierId());
        ui->venSearchEditbox->setText(bean->supplierId() + " - " + venBean->supplieName());

        DepBean *depBean = DepDAO::getDepById(bean->depCode());
        ui->depSearchEditbox->setText(bean->depCode() + " - " + depBean->depName());
        ui->userSearchEditbox->setText(bean->personCode() + " - " + CacheBaBeanCommon::personToName(bean->personCode()));
        ui->memoEdit->setText(bean->memo());
        ui->makerEdit->setText(CacheBaBeanCommon::userToName(bean->createdBy(),1));
        ui->auditEdit->setText(bean->handler());
        ui->rdNoEdit->setText(bean->rdNo());
        ui->apNoEdit->setText(bean->apNo());
        ui->apMoneySpinBox->setValue(bean->apMoeny());
        ui->profitEdit->setText(bean->profitNo());
        ui->profitMoneySpinBox->setValue(bean->profitMoeny());
        QString settleType =  "普通结算";
        if(bean->settleType() == 1) settleType =  "红字结算";
        for(int i = 0; i < ui->busTypeComboBox->count(); i++){
            if(settleType == ui->busTypeComboBox->itemText(i)){
                ui->busTypeComboBox->setCurrentIndex(i);
            }
        }
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
        ui->toolBar->actions().at(3)->setEnabled(false);
        ui->toolBar->actions().at(6)->setEnabled(false);
        ui->toolBar->actions().at(8)->setEnabled(false);

        if(bean->isHandle()){
            ui->toolBar->actions().at(4)->setEnabled(false);
            ui->toolBar->actions().at(5)->setEnabled(true);
        }else{
            ui->toolBar->actions().at(4)->setEnabled(true);
            ui->toolBar->actions().at(5)->setEnabled(false);
        }
        QList<PuSettleVouchsBean *> pBeanList = PuSettleDAO::queryPuSettleVouchsByNo(no);
        mChildVouchsList = PuSettleDAO::queryPuSettleVouchsByNo(no);
        ui->tableWidget->setRowCount(pBeanList.size() + 1);
        mTotalQuantity = 0;
        mTotalSum = 0;
        for(int i =0; i < pBeanList.size(); i++){
            WidgetBeanUtil::setBeanToTableWidget(ui->tableWidget,i,mChildItem,pBeanList.at(i));
            mTotalQuantity = mTotalQuantity + pBeanList.at(i)->quantity();
            mTotalSum = mTotalSum + pBeanList.at(i)->amount();
        }

        mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity,'f',2)));
        mFootModel->setItem(8,new QStandardItem(QString::number(mTotalSum,'f',2)));
        mFootView->reset();
        readAuthority();
    }
}

void PuSettleWin::inputClear(){
    WidgetBeanUtil::clearLayoutWidgetData(ui->headGridLayout);


}

void PuSettleWin::setInputEnable(bool b){
     WidgetBeanUtil::setLayoutWidgetEnable(ui->headGridLayout,b);
     if(b){
         ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);
     }else{
         ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
     }
}

void PuSettleWin::addActionsTo(actionProperties acts[], unsigned int numElems){
    baseToolBar = addToolBar(tr("Base"));
    baseToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    baseToolBar->setIconSize(QSize(24, 24));
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolBar->setIconSize(QSize(24, 24));
    for (unsigned int i = 0; i < numElems; i++){
        QAction *act = new QAction(acts[i].actionName, this);
        QIcon icon;
        icon.addPixmap(acts[i].pixmap,  QIcon::Normal, QIcon::Off);

        act->setText(acts[i].actionTitle);
        act->setObjectName(acts[i].actionName);
        act->setIcon(icon);

        connect(act, SIGNAL(triggered()), this, acts[i].slot);

        if(i < 9){
            act->setEnabled(false);
            ui->toolBar->addAction(act);
            addAction(act);
            QString proceccId = mWinId + QString::number(i);
            HaoKeApp::writeProcess(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::puSubId().toInt(),mWinId.toInt(),proceccId.toInt(),acts[i].toolTip,acts[i].actionName);

        }
        else
            baseToolBar->addAction(act);
    }

    connect(ui->toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolBarActTriggered(QAction*)));


}

void PuSettleWin::toolBarActTriggered(QAction *act){
    //写入操作日志
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::puSubId().toInt(),mWinId.toInt(),act->objectName());

}

void PuSettleWin::sFirst(){
    if(mIsEdit){
        if(QMessageBox::warning(this,"提示","是否放弃保存",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
            mIsEdit = false;
            ui->toolBar->actions().at(0)->setEnabled(true);
            ui->toolBar->actions().at(1)->setEnabled(true);
            ui->toolBar->actions().at(2)->setEnabled(true);
            ui->toolBar->actions().at(3)->setEnabled(false);
            ui->toolBar->actions().at(6)->setEnabled(false);
            ui->toolBar->actions().at(8)->setEnabled(false);
            setInputEnable(false);
        }else{
            return ;
        }
    }
    mRecord = 0;
    ui->tableWidget->clear();
    refreshData();
    QList<PuSettleVouchBean *> vouchBeanList = PuSettleDAO::queryPuSettleByPage(0,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());

    }
    baseToolBar->actions().at(0)->setEnabled(false);
    baseToolBar->actions().at(1)->setEnabled(true);
    baseToolBar->actions().at(2)->setEnabled(false);
    baseToolBar->actions().at(3)->setEnabled(true);
}

void PuSettleWin::sNext(){
    if(mIsEdit){
        if(QMessageBox::warning(this,"提示","是否放弃保存",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
            mIsEdit = false;
            ui->toolBar->actions().at(0)->setEnabled(true);
            ui->toolBar->actions().at(1)->setEnabled(true);
            ui->toolBar->actions().at(2)->setEnabled(true);
            ui->toolBar->actions().at(3)->setEnabled(false);
            ui->toolBar->actions().at(6)->setEnabled(false);
            ui->toolBar->actions().at(8)->setEnabled(false);
            setInputEnable(false);
        }else{
            return ;
        }
    }
    if(mRecord < mCountRecord)
        mRecord = mRecord + 1;
    if(mRecord == mCountRecord){
        baseToolBar->actions().at(3)->setEnabled(false);
        baseToolBar->actions().at(1)->setEnabled(false);
    }
    ui->tableWidget->clear();
    refreshData();
    QList<PuSettleVouchBean *> vouchBeanList = PuSettleDAO::queryPuSettleByPage(mRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(2)->setEnabled(true);
    baseToolBar->actions().at(0)->setEnabled(true);
}

void PuSettleWin::sPrevious(){
    if(mIsEdit){
        if(QMessageBox::warning(this,"提示","是否放弃保存",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
            mIsEdit = false;
            ui->toolBar->actions().at(0)->setEnabled(true);
            ui->toolBar->actions().at(1)->setEnabled(true);
            ui->toolBar->actions().at(2)->setEnabled(true);
            ui->toolBar->actions().at(3)->setEnabled(false);
            ui->toolBar->actions().at(6)->setEnabled(false);
            ui->toolBar->actions().at(8)->setEnabled(false);
            setInputEnable(false);
        }else{
            return ;
        }
    }
    if(mRecord > 0)
        mRecord = mRecord - 1;
    if(mRecord == 0){
        baseToolBar->actions().at(0)->setEnabled(false);
        baseToolBar->actions().at(2)->setEnabled(false);
    }
    ui->tableWidget->clear();
    refreshData();
    QList<PuSettleVouchBean *> vouchBeanList = PuSettleDAO::queryPuSettleByPage(mRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(1)->setEnabled(true);
    baseToolBar->actions().at(3)->setEnabled(true);
}

void PuSettleWin::sEnd(){
    if(mIsEdit){
        if(QMessageBox::warning(this,"提示","是否放弃保存",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
            mIsEdit = false;
            ui->toolBar->actions().at(0)->setEnabled(true);
            ui->toolBar->actions().at(1)->setEnabled(true);
            ui->toolBar->actions().at(2)->setEnabled(true);
            ui->toolBar->actions().at(3)->setEnabled(false);
            ui->toolBar->actions().at(6)->setEnabled(false);
            ui->toolBar->actions().at(8)->setEnabled(false);
            setInputEnable(false);
        }else{
            return ;
        }
    }
    ui->tableWidget->clear();
    refreshData();
    mCountRecord = PuSettleDAO::getPuSettleCount("01") - 1;
    mRecord = mCountRecord;
    QList<PuSettleVouchBean *> vouchBeanList = PuSettleDAO::queryPuSettleByPage(mCountRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(0)->setEnabled(true);
    baseToolBar->actions().at(1)->setEnabled(false);
    baseToolBar->actions().at(2)->setEnabled(true);
    baseToolBar->actions().at(3)->setEnabled(false);
}

void PuSettleWin::sCancel(){

    this->close();
}

void PuSettleWin::closeEvent(QCloseEvent *event){

    if(mIsEdit){
        if(QMessageBox::warning(this,"提示","是否放弃保存",QMessageBox::Yes | QMessageBox::No)==QMessageBox::No){
            event->ignore();
            return ;
        }
    }

    event->accept();
    HaoKeApp::getInstance()->gui()->removeSubWin(this);


}

void PuSettleWin::setupContextMenu(){
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void PuSettleWin::sNew(){
    inputClear();
    mIsEdit = true;
    mEditStatus = addFlag;
    setInputEnable(true);
    ui->dateEdit->setDate(HaoKeApp::getInstance()->getOperateDate());
    ui->toolBar->actions().at(0)->setEnabled(false);
    ui->toolBar->actions().at(1)->setEnabled(false);
    ui->toolBar->actions().at(2)->setEnabled(false);
    ui->toolBar->actions().at(3)->setEnabled(true);
    ui->toolBar->actions().at(4)->setEnabled(false);
    ui->toolBar->actions().at(5)->setEnabled(false);
    ui->toolBar->actions().at(6)->setEnabled(true);
    ui->toolBar->actions().at(8)->setEnabled(true);
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    int nCount = PuSettleDAO::getPuSettleCount(QDate::currentDate()) ;
    mNo = PuItemUtil::getToDayMaxNo(mWinId,"pu_settle_vouch",QDate::currentDate(),nCount);

    ui->noEdit->setText(mNo);
    refreshData();

    mTotalQuantity = 0;
    mTotalSum = 0;

    mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity)));
    mFootModel->setItem(7,new QStandardItem(QString::number(mTotalSum)));
    mFootView->reset();
    readAuthority();
}

void PuSettleWin::sEdit(){
    if(ui->noEdit->text().length() < 1){
        QMessageBox::warning(this,"提示","无单不能修改！");
        return ;
    }
    if(ui->auditEdit->text().length() > 0){
        QMessageBox::warning(this,"提示","已审核，不能修改！");
        return ;
    }
    mIsEdit = true;
    setInputEnable(true);
    mEditStatus = editFlag;
    ui->noEdit->setEnabled(false);

    ui->toolBar->actions().at(0)->setEnabled(false);
    ui->toolBar->actions().at(1)->setEnabled(false);
    ui->toolBar->actions().at(2)->setEnabled(true);
    ui->toolBar->actions().at(3)->setEnabled(true);
    ui->toolBar->actions().at(4)->setEnabled(false);
    ui->toolBar->actions().at(5)->setEnabled(false);
    ui->toolBar->actions().at(6)->setEnabled(true);
    ui->toolBar->actions().at(8)->setEnabled(true);
    readAuthority();
}

void PuSettleWin::sSave(){
    if(ui->noEdit->text().length()< 1){
        QMessageBox::warning(this,"提示","出错，此单号为空，不能保存");
        return ;
    }

    QAbstractItemModel *model = ui->tableWidget->model();
    int rowNum = model->rowCount() -1;
    if(rowNum == 0){
        QMessageBox::warning(this,"提示","无数据，不能保存");
        return;
    }

    if(ui->venSearchEditbox->text().trimmed().length() < 1){
        QMessageBox::warning(this,"提示","请输入供应商");
        return ;
    }

    if(ui->rdNoEdit->text().trimmed().length() < 1){
        QMessageBox::warning(this,"提示","入库单单号不能为空,请提出采购入库单结算！");
        return ;
    }

    for(int i = 0;i < rowNum; i++){
        QString pCode = model->data(model->index(i,0)).toString();
        QString rdNo = ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"rdNo"))).toString();
        if(rdNo != ui->rdNoEdit->text()){
            QMessageBox::warning(this,"提示","结算单只能对应一张入库单");
            return ;
        }
        ProductBean *pBean = ProductDAO::getProductByCode(pCode);
        if(pBean != NULL && pBean->productCode().length() > 0){
            for(int j = 0; j < ui->tableWidget->rowCount(); j ++){
                if(i != j && ui->tableWidget->model()->data(ui->tableWidget->model()->index(j,0)).toString().trimmed() == pCode){
                    QMessageBox::information(this,"提示","此商品：" + pCode + " 重复！");
                    ui->tableWidget->model()->setData(ui->tableWidget->model()->index(j,0),"");
                    return;
                }
            }
        }else{
            QMessageBox::warning(this,"提示","此商品不存在，不能保存");
            return ;
        }
        PuSettleVouchsBean *puVouchsBean = new PuSettleVouchsBean();
        puVouchsBean->setSettleNo(mNo);
        WidgetBeanUtil::setItemModelToBean(model,i,mChildItem,puVouchsBean);
        PuSettleVouchsBean *oBean = PuSettleDAO::getPuSettleRdsBy(puVouchsBean->rdNo(),puVouchsBean->productCode());
        if(oBean && oBean->rdNo().length() > 2){
            double real = oBean->quantity();
            qDebug() << real << " quantity:" << puVouchsBean->quantity();
            if(qAbs(real) - qAbs(puVouchsBean->quantity()) < 0){
                QMessageBox::warning(this,"提示","入库单:"+ puVouchsBean->rdNo() + " ,商品："
                                     + puVouchsBean->productName() + " 可结算数量不足！,最大结算数量为:" + QString::number(real));
                return ;
            }

        }else{
            QMessageBox::warning(this,"提示","入库单号:"+ puVouchsBean->rdNo() + " ,商品："
                                 + puVouchsBean->productName() + " 不存在");
            return ;
        }
    }

    if(mEditStatus == addFlag){
        if(PuSettleDAO::getPuSettleCountByNo(ui->noEdit->text()) > 0){
            QMessageBox::warning(this,"提示","出错，此单号重复，不能保存，重新生成新单号");
            int nCount = PuSettleDAO::getPuSettleCount(QDate::currentDate()) ;
            mNo = PuItemUtil::getToDayMaxNo(mWinId,"pu_settle_vouch",QDate::currentDate(),nCount);

            ui->noEdit->setText(mNo);
            return ;
        }

        mNo = ui->noEdit->text();

        PuSettleVouchBean *puVouchBean = new PuSettleVouchBean();
        puVouchBean->setNo(ui->noEdit->text());
        QDateTime time = QDateTime::currentDateTime();
        puVouchBean->setSettleDate(ui->dateEdit->date());
        if(ui->busTypeComboBox->currentText()=="普通结算")
            puVouchBean->setSettleType(0);
        else puVouchBean->setSettleType(1);
        puVouchBean->setSupplierId(ui->venSearchEditbox->text());
        puVouchBean->setPayCode("");
        puVouchBean->setDepCode(ui->depSearchEditbox->text());
        puVouchBean->setPersonCode(ui->userSearchEditbox->text());
        puVouchBean->setBusType("01");
        puVouchBean->setMemo(ui->memoEdit->text());
        puVouchBean->setHandler("");
        puVouchBean->setCreated(time);
        puVouchBean->setRdNo(ui->rdNoEdit->text());
        puVouchBean->setApNo(ui->apNoEdit->text());
        puVouchBean->setApMoney(ui->apMoneySpinBox->value());
        puVouchBean->setProfitNo(ui->profitEdit->text());
        puVouchBean->setProfitMoney(ui->profitMoneySpinBox->value());
        puVouchBean->setCreatedBy(HaoKeApp::getInstance()->getUserId());
        puVouchBean->setCreater(HaoKeApp::getInstance()->getUserDesc());
        puVouchBean->setUpdated(time);
        puVouchBean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        puVouchBean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        puVouchBean->setStatus(0);
        puVouchBean->setIsHandle(false);
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int mResult = 0;
        mResult = PuSettleDAO::createPuSettle(puVouchBean);
        int cResult = 0;

        for(int i = 0;i < rowNum; i++){
            PuSettleVouchsBean *puVouchsBean = new PuSettleVouchsBean();
            puVouchsBean->setSettleNo(mNo);
            WidgetBeanUtil::setItemModelToBean(model,i,mChildItem,puVouchsBean);
            puVouchsBean->setTaxPrice(puVouchsBean->sPrice());
            int c = PuSettleDAO::createPuSettleVouchs(puVouchsBean);
            if(c < 1){
                DBManager::getDBManager()->getGoinDBConnection().rollback();
                QMessageBox::information(this,"提示","此" + puVouchsBean->rdNo() + " 数据提交出错,不能保存");
                return ;
            }
            cResult = cResult + c;
        }
        if(mResult > 0 && cResult > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            mIsEdit = false;
            ui->toolBar->actions().at(0)->setEnabled(true);
            ui->toolBar->actions().at(1)->setEnabled(true);
            ui->toolBar->actions().at(2)->setEnabled(true);
            ui->toolBar->actions().at(3)->setEnabled(false);
            ui->toolBar->actions().at(6)->setEnabled(false);
            ui->toolBar->actions().at(8)->setEnabled(false);
            ui->makerEdit->setText(CacheBaBeanCommon::userToName(HaoKeApp::getInstance()->getUserId(),1));
            setInputEnable(false);
            mCountRecord = PuSettleDAO::getPuSettleCount("01") - 1;
            mRecord = mCountRecord;

            mDelChildList.clear();
            mAddChildVouchsList.clear();
            mChildVouchsList.clear();

            ui->tableWidget->clear();
            refreshData();
            read(ui->noEdit->text());

            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }else{
            DBManager::getDBManager()->getGoinDBConnection().rollback();
            QMessageBox::information(this,"Prompt","数据提交出错");
            return ;
        }
        readAuthority();


    }else if(mEditStatus == editFlag){
        //修改数据保存
        PuSettleVouchBean *puVouchBean = PuSettleDAO::getPuSettleByNo(mNo);
        if(puVouchBean && puVouchBean->isHandle()){
            QMessageBox::warning(this,"提示","此单已审核，不能修改");
            return ;
        }


        puVouchBean->setNo(ui->noEdit->text());
        QDateTime time = QDateTime::currentDateTime();
        puVouchBean->setSettleDate(ui->dateEdit->date());
        if(ui->busTypeComboBox->currentText()=="普通结算")
            puVouchBean->setSettleType(0);
        else puVouchBean->setSettleType(1);
        puVouchBean->setBusType("01");
        puVouchBean->setSupplierId(ui->venSearchEditbox->text());
        puVouchBean->setDepCode(ui->depSearchEditbox->text());
        puVouchBean->setPersonCode(ui->userSearchEditbox->text());
        puVouchBean->setMemo(ui->memoEdit->text());
        puVouchBean->setUpdated(time);
        puVouchBean->setRdNo(ui->rdNoEdit->text());
        puVouchBean->setApNo(ui->apNoEdit->text());
        puVouchBean->setApMoney(ui->apMoneySpinBox->value());
        puVouchBean->setProfitNo(ui->profitEdit->text());
        puVouchBean->setProfitMoney(ui->profitMoneySpinBox->value());
        puVouchBean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        puVouchBean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int mResult = 0;
        mResult = PuSettleDAO::updatePuSettle(puVouchBean);
        int cResult = 0;
        //删除
        for(int i = 0; i < mDelChildList.size(); i++){
            PuSettleVouchsBean *delBean = new PuSettleVouchsBean;
            delBean->setId(mDelChildList.at(i));
            int c = PuSettleDAO::delPuSettleVouchs(delBean);
            cResult = cResult + c;
        }
        //更新
        for(int i = 0; i < mChildVouchsList.size(); i++){
            mChildVouchsList.at(i)->setTaxPrice(mChildVouchsList.at(i)->sPrice());
            int c = PuSettleDAO::updatePuSettleVouchs(mChildVouchsList.at(i));
            cResult = cResult + c;
        }
        //添加
        for(int i = 0; i < mAddChildVouchsList.size(); i++){
            mAddChildVouchsList.at(i)->setTaxPrice(mAddChildVouchsList.at(i)->sPrice());
            int c = PuSettleDAO::createPuSettleVouchs(mAddChildVouchsList.at(i));
            cResult = cResult + c;
        }

        if(mResult > 0 && cResult>0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            mIsEdit = false;
            ui->toolBar->actions().at(0)->setEnabled(true);
            ui->toolBar->actions().at(1)->setEnabled(true);
            ui->toolBar->actions().at(2)->setEnabled(true);
            ui->toolBar->actions().at(3)->setEnabled(false);
            ui->toolBar->actions().at(6)->setEnabled(false);
            ui->toolBar->actions().at(8)->setEnabled(false);
            ui->makerEdit->setText(CacheBaBeanCommon::userToName(HaoKeApp::getInstance()->getUserId(),1));
            setInputEnable(false);
            mDelChildList.clear();
            mAddChildVouchsList.clear();
            mChildVouchsList.clear();

            ui->tableWidget->clear();
            refreshData();
            read(ui->noEdit->text());

            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }else{
            DBManager::getDBManager()->getGoinDBConnection().rollback();
            QMessageBox::information(this,"Prompt","数据提交出错");
            return ;
        }
        readAuthority();
    }
}

void PuSettleWin::sDelete(){
    if(ui->auditEdit->text().length() > 0){
        QMessageBox::warning(this,"提示","已审核，不能删除！");
        return ;
    }
    if(QMessageBox::warning(this,"提示","是否删除？",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
        PuSettleVouchBean *bean = PuSettleDAO::getPuSettleByNo(mNo);
        bean->setNo(mNo);
        if(bean->isHandle()){
            QMessageBox::warning(this,"提示",bean->handler() + " 已审核，不能删除！");
            return ;
        }
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int mResult = PuSettleDAO::delPuSettleByNo(bean);
        int cResult = PuSettleDAO::delPuSettleVouchsByNo(ui->noEdit->text());
        if(mResult > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            mIsEdit =false;
            setInputEnable(false);
            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
            if(mRecord ==0){
                sNext();
            }else sPrevious();
            mCountRecord = PuSettleDAO::getPuSettleCount("01") - 1;
        }else{
            QMessageBox::critical(this,"错误","删除出错");
            DBManager::getDBManager()->getGoinDBConnection().rollback();
        }
        readAuthority();

    }
}

void PuSettleWin::sAudit(){
    PuSettleVouchBean *bean = PuSettleDAO::getPuSettleByNo(ui->noEdit->text().trimmed());
    QString rdNo = ui->rdNoEdit->text();
    if(bean && bean->no().length() > 0){
        if(bean->isHandle()){
            ui->toolBar->actions().at(4)->setEnabled(false);
            ui->toolBar->actions().at(5)->setEnabled(true);
            QMessageBox::information(this,"提示","已审核，不能再审核！");
            return ;
        }
        RdBean *rdBean = RdDAO::getRdByNo(rdNo);
        if(rdBean && rdBean->rdNo().length()> 2 ){
            if(!rdBean->isHandle()){
                QMessageBox::information(this,"提示","出库单" + rdNo + "未审核,不能审核此结算单！");
                return ;
            }
        }else{
            QMessageBox::information(this,"提示","出库单" + rdNo + "不存在,不能审核此结算单！");
            return ;
        }
        for(int i = 0; i < mChildVouchsList.size(); i++){
            PuSettleVouchsBean *oBean = PuSettleDAO::getPuSettleRdsBy(mChildVouchsList.at(i)->rdNo(),mChildVouchsList.at(i)->productCode());
            if(oBean && oBean->rdNo().length() > 2){
                double real = oBean->quantity();
                //qDebug() << real << " quantity:" << mAddChildVouchsList.at(i)->quantity();

                if(qAbs(real) - qAbs(mChildVouchsList.at(i)->quantity()) < 0){
                    QMessageBox::warning(this,"提示","入库单:"+ mChildVouchsList.at(i)->rdNo() + " ,商品："
                                         + mChildVouchsList.at(i)->productName() + " 可结算数量不足！,最大结算数量为:" + QString::number(real));
                    return ;
                }

            }else{
                QMessageBox::warning(this,"提示","入库单:"+ mChildVouchsList.at(i)->rdNo() + " ,商品："
                                     + mChildVouchsList.at(i)->productName() + " 不存在" );
                return ;
            }
        }
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int sResult = 0;


        bean->setHandler(CacheBaBeanCommon::userToName(HaoKeApp::getInstance()->getUserId(),1));
        bean->setAuditTime(QDateTime::currentDateTime());
        bean->setIsHandle(true);
        bean->setUpdated(QDateTime::currentDateTime());
        bean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        bean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        int result = PuSettleDAO::updatePuSettle(bean);
        if(result > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            ui->auditEdit->setText(bean->handler());
            if(PuSettleDAO::getPuSettleRdsCount(rdNo) == 0){
                RdBean *rdBean = RdDAO::getRdByNo(rdNo);
                if(rdBean && rdBean->rdNo().length()> 2){
                    rdBean->setIsBill(true);
                    rdBean->setBillNo(mNo);
                    RdDAO::updateRd(rdBean);
                }
            }
            ui->toolBar->actions().at(4)->setEnabled(false);
            ui->toolBar->actions().at(5)->setEnabled(true);
            readAuthority();
            QMessageBox::information(this,"提示","审核成功！");
        }else{
            DBManager::getDBManager()->getGoinDBConnection().rollback();
            QMessageBox::information(this,"提示","审核出错！");
        }

    }else{
        QMessageBox::critical(this,"错误","无此单据，不能审核");
    }
}

void PuSettleWin::sAbAudit(){
    PuSettleVouchBean *bean = PuSettleDAO::getPuSettleByNo(ui->noEdit->text().trimmed());
    if(bean !=NULL && bean->no().length() > 0){
        if(!bean->isHandle()){
            ui->toolBar->actions().at(4)->setEnabled(true);
            ui->toolBar->actions().at(5)->setEnabled(false);
            QMessageBox::information(this,"提示","未审核，不能再弃审！");
            return ;
        }

        DBManager::getDBManager()->getGoinDBConnection().transaction();

        bean->setHandler("");
        bean->setIsHandle(false);
        bean->setUpdated(QDateTime::currentDateTime());
        bean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        bean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        int result = PuSettleDAO::updatePuSettle(bean);

        RdBean *rdBean = RdDAO::getRdByNo(ui->rdNoEdit->text());
        if(rdBean && rdBean->rdNo().length()> 2){
            rdBean->setIsBill(false);
            rdBean->setBillNo("");
            result = RdDAO::updateRd(rdBean);
        }
        if(result > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            ui->auditEdit->setText(bean->handler());
            ui->toolBar->actions().at(4)->setEnabled(true);
            ui->toolBar->actions().at(5)->setEnabled(false);
            readAuthority();
            QMessageBox::information(this,"提示","弃审成功！");
        }else{
            DBManager::getDBManager()->getGoinDBConnection().rollback();
            QMessageBox::information(this,"提示","弃审出错！");
        }

    }
}

void PuSettleWin::sDelRow(){
    if(!mIsEdit){
        QMessageBox::information(this,"提示","不能修改！");
        return;
    }
    QAbstractItemModel *modelTotal = ui->tableWidget->model();
    QItemSelectionModel *seleced = ui->tableWidget->selectionModel();
    int row = seleced->currentIndex().row();

    if(row >= 0){

        QString name = modelTotal->data(modelTotal->index(row,1)).toString();
        QString code = modelTotal->data(modelTotal->index(row,0)).toString();
        if(name.length() < 1){
            QMessageBox::information(this,"提示","不能删除此行！");
            return;
        }
        if(QMessageBox::warning(this, tr("Prompt"),"是否要删除此 *" + name + "*",
                                QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
            if(mEditStatus == addFlag){
                modelTotal->removeRow(row);
            }else{
                for(int i =0 ; i < mChildVouchsList.size(); i++){
                    if(code == mChildVouchsList.at(i)->productCode()){
                        mDelChildList.append(mChildVouchsList.at(i)->id());
                        mChildVouchsList.removeAt(i);

                    }
                }
                for(int i =0 ; i < mAddChildVouchsList.size(); i++){
                    if(code == mAddChildVouchsList.at(i)->productCode()){
                        mAddChildVouchsList.removeAt(i);
                    }
                }
                modelTotal->removeRow(row);
            }
            if(mIsEdit){
                mTotalQuantity = 0;
                mTotalSum = 0;
                QAbstractItemModel *model = ui->tableWidget->model();
                for(int i = 0 ; i < ui->tableWidget->rowCount(); i++){
                    double quantity = model->data(model->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"quantity"))).toDouble();
                    double money = model->data(model->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"amount"))).toDouble();

                    mTotalQuantity = mTotalQuantity + quantity;
                    mTotalSum = mTotalSum + money;

                }

                mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity)));
                mFootModel->setItem(8,new QStandardItem(QString::number(mTotalSum)));
                mFootView->reset();

            }


        }
    }else{
        QMessageBox::information(this,"Prompt","请选择要删除的行!");
    }
}

void PuSettleWin::sPrint(){
    QString fileName = "ReportpuSettle.xml";
    QString title = "采购结算单";
    QString formatData = "";
    QList<ReportFormatBean *> rptList = ReportDAO::queryRptBy(mWinId);

    if(rptList.size() > 0){
        formatData = rptList.at(rptList.size() - 1)->reportFormat();
    }else {
        formatData = PrintUtils::makeWinPrintFormatData(ui->headGridLayout,mChildItem,title,fileName);
        ReportFormatBean *rptBean = new ReportFormatBean();
        rptBean->setReportTitle(mWinTitle);
        rptBean->setReportFormat(formatData);
        rptBean->setWinId(mWinId.toInt());
        rptBean->setCreated(QDateTime::currentDateTime());
        rptBean->setCreatedBy(HaoKeApp::getInstance()->getUserId());
        rptBean->setCreater(HaoKeApp::getInstance()->getUserDesc());
        int result = ReportDAO::createRpt(rptBean);

    }
    QtRPT *report = new QtRPT(this);
    report->setBackgroundImage(QPixmap("./qt_background_portrait.png"));
    connect(report,SIGNAL(setValue(int&,QString&,QVariant&,int)),this,SLOT(setValue(int&,QString&,QVariant&,int)));
    report->recordCount << ui->tableWidget->rowCount() - 1;
    if (report->loadReportFormat(formatData) == false) {
        qDebug()<<"Report's format is null";
    }
    report->setWindowTitle(title);
    report->setModel(ui->tableWidget->model());
    report->setChildItem(mChildItem);

    if(HaoKeApp::getInstance()->getUserLevel()>8){
        DesignOptDialog *designOptDlg = new DesignOptDialog(this);
        QString formatData2 = PrintUtils::makeWinPrintFormatData(ui->headGridLayout,mChildItem,title,fileName);
        designOptDlg->setPrintData(0,mWinId,title,formatData,formatData2);

        int r = designOptDlg->exec();
        if(r == QDialog::Accepted){
            ReportFormatBean *fBean = ReportDAO::getRptById(QString::number(designOptDlg->getPrintId()));
            if(fBean){
                formatData = fBean->reportFormat();
                if (report->loadReportFormat(formatData) == false) {
                    qDebug()<<"Report's file not found";
                }
            }
            report->printExec();
        }else{

        }
    }else{
        report->printExec();
    }

}

void PuSettleWin::sChoose(){
    if(ui->venSearchEditbox->text().trimmed().length() < 1){
        QMessageBox::warning(this,"提示","请输入供应商");
        return ;
    }
    ChooseVouchDialog *cVouchDlg = new ChooseVouchDialog(this);
    cVouchDlg->setWindowTitle("提取采购入库单");

    childItemProperties childItem[] = {
            { "rd_no","入库单号","入库单号", 0,100, true, true },
            { "bus_date","入库日期","入库日期", 1,80, true, true },
            { "bus_type","单据类型","单据类型", 2,80, true, false },
            { "supplier_id","供应商编码","供应商编码", 3,80, true, false },
            { "supplie_name","供应商名称","供应商名称", 4, 120,true, true },
            { "product_code","商品编码","商品编码", 5,80, true, true },
            { "product_name","商品名称","商品名称", 6,100, true, true },
            { "product_style","规格","商品",7,80, true, false },
            { "unit","采购单位","采购单位", 8,60, true, false },
            { "quantity","入库数量","入库数量", 9, 80,true, true },
            { "not_quantity","可结数量","可结数量", 10, 80,true, true },
            { "unit_price","采购单价","采购单价", 11,80, true, true },
            { "money","采购金额","采购金额", 12,80, true, false }

    };


    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    QList<SupplieBean *> venBeanList = SupplieDAO::querySupplie();
    QStringList venList ;
    for(int i =0 ; i < venBeanList.size(); i++){
        venList << venBeanList.at(i)->supplieCode() + " - " +venBeanList.at(i)->supplieName();
    }
    QDate oneDate = QDate(HaoKeApp::getInstance()->getOperateDate().year(),HaoKeApp::getInstance()->getOperateDate().month(),1);
    cVouchDlg->sDateEdit1->setDate(oneDate);
    cVouchDlg->dwNameQks->setListModel(true);
    cVouchDlg->dwNameQks->setItems(venList);
    SupplieBean *venBean = SupplieDAO::getSupplieByCode(ui->venSearchEditbox->text());
    cVouchDlg->dwNameQks->setText(venBean->supplieCode() + " - " + venBean->supplieName());
    cVouchDlg->dwNameQks->setReadOnly(true);
    QList<SupplieTypeBean *> typeBeanList = SupplieTypeDAO::queryType();
    QStringList typeList ;
    for(int i =0 ; i < typeBeanList.size(); i++){
        typeList << typeBeanList.at(i)->typeId() + " - " +typeBeanList.at(i)->typeName();
    }
    cVouchDlg->dwTypeQks->setListModel(true);
    cVouchDlg->dwTypeQks->setItems(typeList);
    cVouchDlg->dwTypeQks->setReadOnly(true);
    QString sql = " SELECT a.id, a.rd_no, a.bus_date, a.bus_type, a.supplier_id, b.supplie_name, a.wh_code,a.dep_code,c.product_code,"
                " d.product_name, c.product_style, c.unit, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
                " a.status, a.memo, c.quantity,c.not_quantity , c.unit_price,c.money "
                " from rd_record a, "
                    " ( SELECT g.*,g.quantity-COALESCE(h.sum_qunatity,0) as not_quantity "
                    " from rd_records g "
                    " LEFT JOIN ( "
                        " SELECT e.rd_no,e.product_code,sum(quantity) as sum_qunatity "
                        " from pu_settle_vouchs e "
                        " GROUP BY e.rd_no,e.product_code) h "
                    " ON g.rd_no=h.rd_no AND g.product_code=h.product_code "
                    " where g.quantity-COALESCE(h.sum_qunatity,0) > 0 "
                " ) c,ba_product d,ba_suppliers b "
                " WHERE a.bus_date>=? AND a.bus_date<=? AND b.supplie_type like ? AND a.supplier_id like ? "
                " AND d.type_id like ? AND c.product_code like ?"
                " AND a.rd_no=c.rd_no AND a.is_handle=true AND c.product_code=d.product_code AND a.supplier_id= b.supplie_code "
                " AND a.vouch_type='01' ORDER BY id ASC";
    if(ui->busTypeComboBox->currentText() == "红字结算"){
        sql = " SELECT a.id, a.rd_no, a.bus_date, a.bus_type, a.supplier_id, b.supplie_name, a.wh_code,a.dep_code,c.product_code,"
                        " d.product_name, c.product_style, c.unit, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
                        " a.status, a.memo, c.quantity,c.not_quantity , c.unit_price,c.money "
                        " from rd_record a, "
                            " ( SELECT g.*,g.quantity-COALESCE(h.sum_qunatity,0) as not_quantity "
                            " from rd_records g "
                            " LEFT JOIN ( "
                                " SELECT e.rd_no,e.product_code,sum(quantity) as sum_qunatity "
                                " from pu_settle_vouchs e "
                                " GROUP BY e.rd_no,e.product_code) h "
                            " ON g.rd_no=h.rd_no AND g.product_code=h.product_code "
                            " where (-g.quantity)-(-COALESCE(h.sum_qunatity,0)) > 0 "
                        " ) c,ba_product d,ba_suppliers b "
                        " WHERE a.bus_date>=? AND a.bus_date<=? AND b.supplie_type like ? AND a.supplier_id like ? "
                        " AND d.type_id like ? AND c.product_code like ?"
                        " AND a.rd_no=c.rd_no AND a.is_handle=true AND c.product_code=d.product_code AND a.supplier_id= b.supplie_code "
                        " AND a.vouch_type='14' ORDER BY id ASC";
    }
    cVouchDlg->initData(dItem,sql);
    cVouchDlg->initTableHead();

    if(cVouchDlg->exec() == QDialog::Accepted){
        QItemSelectionModel *selectionModel = cVouchDlg->tableView->selectionModel();
        QModelIndexList selected= selectionModel->selectedIndexes();
        if(selected.size()<1){
            QMessageBox::information(this,"提示","未提取数据");

        }else{
            QAbstractItemModel *modelChoose = cVouchDlg->tableView->model();
            int cusRow = 0;
            QList<int > selectedRowsList;
            foreach (QModelIndex index, selected){
                int b = true;
                for(int i = 0; i < selectedRowsList.size(); i++){
                    if(selectedRowsList.at(i)== index.row())b = false;
                }
                if(selectedRowsList.size() < 1 || b){
                    selectedRowsList.append(index.row());
                }


            }
            mIsChoose = true;
            foreach (int row, selectedRowsList){
                cusRow = ui->tableWidget->rowCount() -1 ;
                QString rdNo = modelChoose->data(modelChoose->index(row,0)).toString();
                QString productCode = modelChoose->data(modelChoose->index(row,5)).toString();
                double quantity = modelChoose->data(modelChoose->index(row,10)).toDouble();
                double price = modelChoose->data(modelChoose->index(row,11)).toDouble();
                double money = modelChoose->data(modelChoose->index(row,12)).toDouble();
                ui->rdNoEdit->setText(rdNo);
                WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"productCode",productCode);
                productCode = ui->tableWidget->model()->data(ui->tableWidget->model()->index(cusRow,WidgetBeanUtil::getChildItemId(mChildItem,"productCode"))).toString();
                if(productCode.trimmed().length() > 1){
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"price",price);
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"sPrice",price);
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"quantity",quantity);
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"amount",money);
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"rdNo",rdNo);
                }

            }
            mIsChoose = false;



        }

    }
}

void PuSettleWin::setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage){
    Q_UNUSED(reportPage);
    QGridLayout *layout = ui->headGridLayout;
    for(int i = 0 ; i < layout->rowCount(); i ++){
        for(int j =0 ; j < layout->columnCount(); j ++ ){
            QLayoutItem *layoutItem = layout->itemAtPosition(i,j);

            if(layoutItem && !layoutItem->isEmpty()){
                QWidget *widget = layoutItem->widget();

                QString className = widget->metaObject()->className();
                if(className == "QLineEdit"){
                    QLineEdit *c = (QLineEdit *)layoutItem->widget();
                    if (paramName == c->objectName()){
                        paramValue = c->text();
                        return ;
                    }
                }else if(className == "QkSearchEditBox"){
                    QkSearchEditBox *c = (QkSearchEditBox *)layoutItem->widget();
                    if (paramName == c->objectName()){
                        //paramValue = c->text() + "  " + c->subText();
                        paramValue = c->subText();
                        return ;
                    }
                }else if(className == "QDateEdit"){
                    QDateEdit *c = (QDateEdit *)layoutItem->widget();
                    if (paramName == c->objectName()){
                        paramValue = c->text();
                        return ;
                    }
                }else if(className == "QComboBox"){
                    QComboBox *c = (QComboBox *)layoutItem->widget();
                    if (paramName == c->objectName()){
                        paramValue = c->currentText();
                        return ;
                    }
                }
            }

        }
    }

}


void PuSettleWin::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if(item->column() == 0 && mIsEdit){
        QString code = item->data(Qt::EditRole).toString().trimmed();
        if(code.length() < 1){
            return ;
        }
        //重复判断
        for(int i = 0; i < ui->tableWidget->rowCount(); i ++){
            if(i != item->row() && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,0)).toString().trimmed() == code){
                QMessageBox::information(this,"提示","此商品,编码：" + code + " 已输入！");
                ui->tableWidget->model()->setData(ui->tableWidget->model()->index(item->row(),0),"");
                return;
            }
        }

        //不可直接录入商品数据
        if(true && !mIsChoose){
            QMessageBox::warning(this,"提示","结算不可直接录入商品,请提取入库单据！");
            ui->tableWidget->model()->setData(ui->tableWidget->model()->index(item->row(),0),"");
            return ;
        }
        //根据ID查询原料信息
        ProductBean *bean = ProductDAO::getProductByCode(code);
        if(code.length() > 0 && bean != NULL && bean->productCode() == code ){
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"productName",bean->productName());
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"productStyle",bean->style());
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"unit",bean->purchaseUnit());
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"price",bean->puPrice());
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"sPrice",bean->puPrice());

            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"quantity",1);
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"amount",1*bean->puPrice());

            if((ui->tableWidget->rowCount()-1) == item->row()){
                QAbstractItemModel *model = ui->tableWidget->model();
                int id = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"id"))).toInt();
                if(mEditStatus == editFlag && id < 1){

                    //单据修改 增加数据
                    PuSettleVouchsBean *puVouchsBean = new PuSettleVouchsBean();
                    puVouchsBean->setSettleNo(ui->noEdit->text());
                    WidgetBeanUtil::setItemModelToBean(model,item->row(),mChildItem,puVouchsBean);
                    puVouchsBean->setTaxPrice(puVouchsBean->sPrice());
                    mAddChildVouchsList.append(puVouchsBean);
                }
                ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            }
        }else{
            QMessageBox::information(this,"提示","不存在此商品");

            return ;
        }

    }else if(item->column() == 4 || item->column()==6){

        QTableWidgetItem *moneyItemR = new QTableWidgetItem();

        double price = ui->tableWidget->model()->data(ui->tableWidget->model()->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"sPrice"))).toDouble();
        double quantity = ui->tableWidget->model()->data(ui->tableWidget->model()->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"quantity"))).toDouble();
        double money = price * quantity;
        moneyItemR->setData(Qt::EditRole,QString::number(money,'f',2));
        ui->tableWidget->setItem(item->row(),7,moneyItemR);
        if(mIsEdit){
            mTotalQuantity = 0;
            mTotalSum = 0;
            QAbstractItemModel *model = ui->tableWidget->model();
            for(int i = 0 ; i < ui->tableWidget->rowCount(); i++){
                double quantity = model->data(model->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"quantity"))).toDouble();
                double money = model->data(model->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"amount"))).toDouble();

                mTotalQuantity = mTotalQuantity + quantity;
                mTotalSum = mTotalSum + money;

            }

            mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity,'f',2)));
            mFootModel->setItem(8,new QStandardItem(QString::number(mTotalSum,'f',2)));
            mFootView->reset();

        }

    }

    //单据修改
    if(mEditStatus == editFlag){
        QAbstractItemModel *model = ui->tableWidget->model();
        int id = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"id"))).toInt();
        QString pCode = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"productCode"))).toString();
        double price = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"sPrice"))).toDouble();
        double quantity = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"quantity"))).toDouble();
        QString rdNo = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"rdNo"))).toString();

        if(item->column() == 4 || item->column()==6 || item->column()==8){
            if(id < 1 ){
                for(int i =0; i < mAddChildVouchsList.size(); i++){
                    if(mAddChildVouchsList.at(i)->productCode() == pCode){
                        double money = price * quantity;
                        \
                        mAddChildVouchsList.at(i)->setQuantity(quantity);
                        mAddChildVouchsList.at(i)->setSPrice(price);
                        mAddChildVouchsList.at(i)->setTaxPrice(price);
                        mAddChildVouchsList.at(i)->setAmount(money);
                        mAddChildVouchsList.at(i)->setRdNo(rdNo);

                    }

                }
            }
        }
        QString name = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"productName"))).toString();
        //单据修改 更新数据
        for(int i =0; i < mChildVouchsList.size(); i++){
            if(mChildVouchsList.at(i)->id() == id){
                double money = price * quantity;
                WidgetBeanUtil::setItemModelToBean(model,item->row(),mChildItem,mChildVouchsList.at(i));

                //mChildVouchsList.at(i)->setMoney(money);

            }
        }

    }
}

