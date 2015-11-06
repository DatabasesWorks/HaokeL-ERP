#include "sasettlewin.h"
#include "ui_sasettlewin.h"
#include "haokeapp.h"
#include "bean/DepBean.h"
#include "bean/SupplieBean.h"
#include "bean/CustomerBean.h"
#include "bean/CustomerTypeBean.h"
#include "bean/EmployeeBean.h"
#include "bean/WareHouseBean.h"
#include "bean/ProductBean.h"
#include "bean/SystemSetBean.h"
#include "dao/depdao.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "dao/employeedao.h"
#include "dao/customerdao.h"
#include "dao/waredao.h"
#include "dao/customertypedao.h"
#include "dao/productdao.h"
#include "dao/SaSettledao.h"
#include "dao/rddao.h"
#include "dao/systemsetdao.h"
#include "common/cachebabeancommon.h"
#include "common/itemutil.h"
#include "common/printutils.h"
#include "qtrpt.h"
#include "bean/ReportFormatBean.h"
#include "dao/reportdao.h"
#include "widget/designoptdialog.h"
#include "widget/choosevouchdialog.h"
#include "saplugin.h"

SaSettleWin::SaSettleWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SaSettleWin)
{
    ui->setupUi(this);
    init();
    //qApp->setStyle("Plastique");
    setupContextMenu();
    mCountRecord = SaSettleDAO::getSaSettleCount("01") - 1;
    mRecord = mCountRecord;
    if(mCountRecord < 0) mCountRecord = 0;
    QList<SaSettleVouchBean *> vouchBeanList = SaSettleDAO::querySaSettleByPage(mCountRecord,1,"01");
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

SaSettleWin::SaSettleWin(QString no,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SaSettleWin)
{
    ui->setupUi(this);
    init();
    setupContextMenu();
    mCountRecord = SaSettleDAO::getSaSettleCount("01") - 1;
    if(mCountRecord < 0) mCountRecord = 0;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(7)->setEnabled(true);

    SaSettleVouchBean *bean = SaSettleDAO::getSaSettleByNo(no);
    if(bean != NULL && bean->no().length() > 0){
        mRecord = SaSettleDAO::getSaSettleRowNumber(bean->id()) - 1;
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
        QMessageBox::critical(this,"����","�˵������ڣ�");
    }
    readAuthority();
}

SaSettleWin::~SaSettleWin()
{
    delete ui;
}

void SaSettleWin::readAuthority(){
    LimitsCommon::authority(ui->toolBar,mWinId.toInt());
}

void SaSettleWin::init(){

    //����ID��
    mWinId = HaoKeApp::saSubId() + SaPlugin::settleWinId();
    mWinTitle = "���۽��㵥";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::saSubId().toInt(),mWinId.toInt(),0,"");

    //���ܲ˵�
    actionProperties actsBase[]= {
        { "SaSettle.new",tr("&New"),SLOT(sNew()), NULL, "New", QPixmap(":/image/Add.png"), NULL,  true, tr("New") },
        { "SaSettle.edit",tr("&Edit"),SLOT(sEdit()), NULL, "Edit", QPixmap(":/image/edit.png"), NULL,  true, tr("Edit") },
        { "SaSettle.delete",tr("&Delete"),SLOT(sDelete()), NULL, "Delete", QPixmap(":/image/cross_48.png"), NULL,  true, tr("Delete") },
        { "SaSettle.save",tr("&Save"),SLOT(sSave()), NULL, "Save", QPixmap(":/image/save.png"), NULL,  true, tr("Save") },
        { "SaSettle.audit",tr("&Audit"),SLOT(sAudit()), NULL, "Audit", QPixmap(":/image/stamp_48.png"), NULL,  true, tr("Audit") },
        { "SaSettle.abandonAudit",tr("&Abandon Audit"),SLOT(sAbAudit()), NULL, "Abandon Audit", QPixmap(":/image/c_stamp_48.png"), NULL,  true, tr("Abandon Audit") },
        { "SaSettle.delRow",tr("&DelRow"),SLOT(sDelRow()), NULL, "DelRow", QPixmap(":/image/minus.png"), NULL,  true, tr("DelRow") },
        { "SaSettle.print",tr("&Print"),SLOT(sPrint()), NULL, "Print", QPixmap(":/image/printer.png"), NULL,  true, tr("Print") },
        { "SaSettle.choose",tr("&Choose"),SLOT(sChoose()), NULL, "Choose", QPixmap(":/image/page_accept.png"), NULL,  true, tr("Choose") },
        { "SaSettle.first",tr("&First"),SLOT(sFirst()), NULL, "First", QPixmap(":/image/first48.png"), NULL,  true, tr("First") },
        { "SaSettle.next",tr("&Next"),SLOT(sNext()), NULL, "Next", QPixmap(":/image/n-next48.png"), NULL,  true, tr("Next") },
        { "SaSettle.previous",tr("&Previous"),SLOT(sPrevious()), NULL, "Previous", QPixmap(":/image/n-previous48.png"), NULL,  true, tr("Previous") },
        { "SaSettle.end",tr("&End"),SLOT(sEnd()), NULL, "End", QPixmap(":/image/end48.png"), NULL,  true, tr("End") },
        { "SaSettle.cancel",tr("&Cancel"),SLOT(sCancel()), NULL, "Cancel", QPixmap(":/image/cancel.png"), NULL,  true, tr("Cancel") }

    };
    addActionsTo(actsBase,sizeof(actsBase) / sizeof(actsBase[0]));

    mIsEdit =false;

    ui->dateEdit->setDate(HaoKeApp::getInstance()->getOperateDate());
    mIsChoose = false;

    //��ʼ�������ʽ
    setInputEnable(false);

    childItemProperties childItem[] = {
        { "productCode","��Ʒ����","��Ʒ����", 0,180, true, true },
        { "productName","��Ʒ����","��Ʒ����", 1,180, true, true },
        { "productStyle","��Ʒ���","��Ʒ���", 2,80, true, false },
        { "unit","��λ","��λ", 3,80, true, false },
        { "quantity","��������","��������", 4, 60,true, true },
        { "price","ԭ����","ԭ����", 5,80, true, true },
        { "sPrice","���㵥��","���㵥��", 6,80, true, true },
        { "amount","������","������", 7,80, true, false },
        { "rdNo","���ⵥ��","���ⵥ��", 8,100, true, false },
        { "id","ID","ID", 9,60, false, false },



    };
    int size = sizeof(childItem) / sizeof(childItem[0]);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(size);
    //qDebug() << size;
    for(int i =0 ; i < size ; i ++){
        mChildItem << childItem[i];
    }
     //��ʼ����β
    mFootView = new HierarchicalHeaderView(Qt::Horizontal);
    mFootView->setClickable(true);
    //mFootView->setMovable(false);
    mFootView->setResizeMode(QHeaderView::Fixed);
    mFootView->setHighlightSections(true);
    mFootModel = new FootTableModel(1,mChildItem.size(),this);
    mFootModel->setItem(0,new QStandardItem(""));
    mFootModel->setItem(1,new QStandardItem("�ϼ�:"));
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
    mFootView->setTextElideMode (Qt::ElideLeft); //...Ч��


    //��ʼ����ͷ����
    QStringList  busTypeList;
    busTypeList << "��ͨ����";
    busTypeList << "���ֽ���";
    ui->busTypeComboBox->addItems(busTypeList);
    QList<CustomerBean *> cusBeanList = CustomerDAO::queryCus();
    QStringList venList ;
    for(int i =0 ; i < cusBeanList.size(); i++){
        venList << cusBeanList.at(i)->cusCode() + " - " +cusBeanList.at(i)->cusName();
    }
    ui->cusQks->setListModel(true);
    ui->cusQks->setItems(venList);

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
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(22);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setItemDelegateForColumn(0,delegate);

}

void SaSettleWin::footResized(int logicalIndex, int oldSize, int newSize){
    mFootView->resizeSection(logicalIndex + 1,newSize);
}

void SaSettleWin::refreshData(){

    QStringList headerLabels;
    for (int i = 0; i < mChildItem.size(); i++){
        headerLabels.append(mChildItem[i].cnName);
        ui->tableWidget->setColumnWidth(mChildItem[i].id,mChildItem[i].width);
        mFootView->resizeSection(mChildItem[i].id + 1,mChildItem[i].width);
        if(!mChildItem[i].visible)ui->tableWidget->hideColumn(mChildItem[i].id);
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
}

void SaSettleWin::read(QString no){
    SaSettleVouchBean *bean = SaSettleDAO::getSaSettleByNo(no);
    mNo = no;
    if(bean != NULL && bean->no().length() > 0){
        ui->noEdit->setText(bean->no());
        ui->dateEdit->setDate(bean->settleDate());
        CustomerBean *venBean = CustomerDAO::getCusByCode(bean->cusCode());
        ui->cusQks->setText(bean->cusCode() + " - " + venBean->cusName());

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
        QString settleType =  "��ͨ����";
        if(bean->settleType() == 1) settleType =  "���ֽ���";
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
        QList<SaSettleVouchsBean *> pBeanList = SaSettleDAO::querySaSettleVouchsByNo(no);
        mChildVouchsList = SaSettleDAO::querySaSettleVouchsByNo(no);
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

void SaSettleWin::inputClear(){
    WidgetBeanUtil::clearLayoutWidgetData(ui->headGridLayout);


}

void SaSettleWin::setInputEnable(bool b){
     WidgetBeanUtil::setLayoutWidgetEnable(ui->headGridLayout,b);
     if(b){
         ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);
     }else{
         ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
     }
}

void SaSettleWin::addActionsTo(actionProperties acts[], unsigned int numElems){
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
            HaoKeApp::writeProcess(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::saSubId().toInt(),mWinId.toInt(),proceccId.toInt(),acts[i].toolTip,acts[i].actionName);

        }
        else
            baseToolBar->addAction(act);
    }

    connect(ui->toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolBarActTriggered(QAction*)));
}

void SaSettleWin::toolBarActTriggered(QAction *act){
    //д�������־
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::saSubId().toInt(),mWinId.toInt(),act->objectName());

}

void SaSettleWin::sFirst(){
    if(mIsEdit){
        if(QMessageBox::warning(this,"��ʾ","�Ƿ��������",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
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
    QList<SaSettleVouchBean *> vouchBeanList = SaSettleDAO::querySaSettleByPage(0,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());

    }
    baseToolBar->actions().at(0)->setEnabled(false);
    baseToolBar->actions().at(1)->setEnabled(true);
    baseToolBar->actions().at(2)->setEnabled(false);
    baseToolBar->actions().at(3)->setEnabled(true);
}

void SaSettleWin::sNext(){
    if(mIsEdit){
        if(QMessageBox::warning(this,"��ʾ","�Ƿ��������",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
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
    QList<SaSettleVouchBean *> vouchBeanList = SaSettleDAO::querySaSettleByPage(mRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(2)->setEnabled(true);
    baseToolBar->actions().at(0)->setEnabled(true);
}

void SaSettleWin::sPrevious(){
    if(mIsEdit){
        if(QMessageBox::warning(this,"��ʾ","�Ƿ��������",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
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
    QList<SaSettleVouchBean *> vouchBeanList = SaSettleDAO::querySaSettleByPage(mRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(1)->setEnabled(true);
    baseToolBar->actions().at(3)->setEnabled(true);
}

void SaSettleWin::sEnd(){
    if(mIsEdit){
        if(QMessageBox::warning(this,"��ʾ","�Ƿ��������",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
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
    mCountRecord = SaSettleDAO::getSaSettleCount("01") - 1;
    mRecord = mCountRecord;
    QList<SaSettleVouchBean *> vouchBeanList = SaSettleDAO::querySaSettleByPage(mCountRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(0)->setEnabled(true);
    baseToolBar->actions().at(1)->setEnabled(false);
    baseToolBar->actions().at(2)->setEnabled(true);
    baseToolBar->actions().at(3)->setEnabled(false);
}

void SaSettleWin::sCancel(){

    this->close();
}

void SaSettleWin::closeEvent(QCloseEvent *event){

    if(mIsEdit){
        if(QMessageBox::warning(this,"��ʾ","�Ƿ��������",QMessageBox::Yes | QMessageBox::No)==QMessageBox::No){
            event->ignore();
            return ;
        }
    }

    event->accept();
    HaoKeApp::getInstance()->gui()->removeSubWin(this);


}

void SaSettleWin::setupContextMenu(){
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void SaSettleWin::sNew(){
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
    int nCount = SaSettleDAO::getSaSettleCount(QDate::currentDate()) ;
    mNo = ItemUtil::getToDayMaxNo(mWinId,"sa_settle_vouch",QDate::currentDate(),nCount);

    ui->noEdit->setText(mNo);
    refreshData();

    mTotalQuantity = 0;
    mTotalSum = 0;

    mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity)));
    mFootModel->setItem(7,new QStandardItem(QString::number(mTotalSum)));
    mFootView->reset();
    readAuthority();
}

void SaSettleWin::sEdit(){
    if(ui->noEdit->text().length() < 1){
        QMessageBox::warning(this,"��ʾ","�޵������޸ģ�");
        return ;
    }
    if(ui->auditEdit->text().length() > 0){
        QMessageBox::warning(this,"��ʾ","����ˣ������޸ģ�");
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

void SaSettleWin::sSave(){
    if(ui->noEdit->text().length()< 1){
        QMessageBox::warning(this,"��ʾ","�����˵���Ϊ�գ����ܱ���");
        return ;
    }

    QAbstractItemModel *model = ui->tableWidget->model();
    int rowNum = model->rowCount() -1;
    if(rowNum == 0){
        QMessageBox::warning(this,"��ʾ","�����ݣ����ܱ���");
        return;
    }

    if(ui->cusQks->text().trimmed().length() < 1){
        QMessageBox::warning(this,"��ʾ","������ͻ�");
        return ;
    }

    if(ui->rdNoEdit->text().trimmed().length() < 1){
        QMessageBox::warning(this,"��ʾ","���ⵥ���Ų���Ϊ��,��������۳��ⵥ���㣡");
        return ;
    }

    for(int i = 0;i < rowNum; i++){
        QString pCode = model->data(model->index(i,0)).toString();
        QString rdNo = ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"rdNo"))).toString();
        if(rdNo != ui->rdNoEdit->text()){
            QMessageBox::warning(this,"��ʾ","���㵥ֻ�ܶ�Ӧһ�ų��ⵥ");
            return ;
        }
        ProductBean *pBean = ProductDAO::getProductByCode(pCode);
        if(pBean != NULL && pBean->productCode().length() > 0){
            for(int j = 0; j < ui->tableWidget->rowCount(); j ++){
                if(i != j && ui->tableWidget->model()->data(ui->tableWidget->model()->index(j,0)).toString().trimmed() == pCode){
                    QMessageBox::information(this,"��ʾ","����Ʒ��" + pCode + " �ظ���");
                    ui->tableWidget->model()->setData(ui->tableWidget->model()->index(j,0),"");
                    return;
                }
            }
        }else{
            QMessageBox::warning(this,"��ʾ","����Ʒ�����ڣ����ܱ���");
            return ;
        }
        SaSettleVouchsBean *puVouchsBean = new SaSettleVouchsBean();
        puVouchsBean->setSettleNo(mNo);
        WidgetBeanUtil::setItemModelToBean(model,i,mChildItem,puVouchsBean);
        SaSettleVouchsBean *oBean = SaSettleDAO::getSaSettleRdsBy(puVouchsBean->rdNo(),puVouchsBean->productCode());
        if(oBean && oBean->rdNo().length() > 2){
            double real = oBean->quantity();
            qDebug() << real << " quantity:" << puVouchsBean->quantity();
            if(qAbs(real) - qAbs(puVouchsBean->quantity()) < 0){
                QMessageBox::warning(this,"��ʾ","���ⵥ:"+ puVouchsBean->rdNo() + " ,��Ʒ��"
                                     + puVouchsBean->productName() + " �ɽ����������㣡,����������Ϊ:" + QString::number(real));
                return ;
            }

        }else{
            QMessageBox::warning(this,"��ʾ","���ⵥ��:"+ puVouchsBean->rdNo() + " ,��Ʒ��"
                                 + puVouchsBean->productName() + " ������");
            return ;
        }
    }

    if(mEditStatus == addFlag){
        if(SaSettleDAO::getSaSettleCountByNo(ui->noEdit->text()) > 0){
            QMessageBox::warning(this,"��ʾ","�����˵����ظ������ܱ��棬���������µ���");
            int nCount = SaSettleDAO::getSaSettleCount(QDate::currentDate()) ;
            mNo = ItemUtil::getToDayMaxNo(mWinId,"sa_settle_vouch",QDate::currentDate(),nCount);

            ui->noEdit->setText(mNo);
            return ;
        }

        mNo = ui->noEdit->text();

        SaSettleVouchBean *puVouchBean = new SaSettleVouchBean();
        puVouchBean->setNo(ui->noEdit->text());
        QDateTime time = QDateTime::currentDateTime();
        puVouchBean->setSettleDate(ui->dateEdit->date());
        if(ui->busTypeComboBox->currentText()=="��ͨ����")
            puVouchBean->setSettleType(0);
        else puVouchBean->setSettleType(1);
        puVouchBean->setCusCode(ui->cusQks->text());
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
        mResult = SaSettleDAO::createSaSettle(puVouchBean);
        int cResult = 0;

        for(int i = 0;i < rowNum; i++){
            SaSettleVouchsBean *puVouchsBean = new SaSettleVouchsBean();
            puVouchsBean->setSettleNo(mNo);
            WidgetBeanUtil::setItemModelToBean(model,i,mChildItem,puVouchsBean);
            puVouchsBean->setTaxPrice(puVouchsBean->sPrice());
            int c = SaSettleDAO::createSaSettleVouchs(puVouchsBean);
            if(c < 1){
                DBManager::getDBManager()->getGoinDBConnection().rollback();
                QMessageBox::information(this,"��ʾ","��" + puVouchsBean->rdNo() + " �����ύ����,���ܱ���");
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
            mCountRecord = SaSettleDAO::getSaSettleCount("01") - 1;
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
            QMessageBox::information(this,"Prompt","�����ύ����");
            return ;
        }
        readAuthority();


    }else if(mEditStatus == editFlag){
        //�޸����ݱ���
        SaSettleVouchBean *puVouchBean = SaSettleDAO::getSaSettleByNo(mNo);
        if(puVouchBean && puVouchBean->isHandle()){
            QMessageBox::warning(this,"��ʾ","�˵�����ˣ������޸�");
            return ;
        }


        puVouchBean->setNo(ui->noEdit->text());
        QDateTime time = QDateTime::currentDateTime();
        puVouchBean->setSettleDate(ui->dateEdit->date());
        if(ui->busTypeComboBox->currentText()=="��ͨ����")
            puVouchBean->setSettleType(0);
        else puVouchBean->setSettleType(1);
        puVouchBean->setCusCode(ui->cusQks->text());
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
        mResult = SaSettleDAO::updateSaSettle(puVouchBean);
        int cResult = 0;
        //ɾ��
        for(int i = 0; i < mDelChildList.size(); i++){
            SaSettleVouchsBean *delBean = new SaSettleVouchsBean;
            delBean->setId(mDelChildList.at(i));
            int c = SaSettleDAO::delSaSettleVouchs(delBean);
            cResult = cResult + c;
        }
        //����
        for(int i = 0; i < mChildVouchsList.size(); i++){
            mChildVouchsList.at(i)->setTaxPrice(mChildVouchsList.at(i)->sPrice());
            int c = SaSettleDAO::updateSaSettleVouchs(mChildVouchsList.at(i));
            cResult = cResult + c;
        }
        //���
        for(int i = 0; i < mAddChildVouchsList.size(); i++){
            mAddChildVouchsList.at(i)->setTaxPrice(mAddChildVouchsList.at(i)->sPrice());
            int c = SaSettleDAO::createSaSettleVouchs(mAddChildVouchsList.at(i));
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
            QMessageBox::information(this,"Prompt","�����ύ����");
            return ;
        }
        readAuthority();
    }
}

void SaSettleWin::sDelete(){
    if(ui->auditEdit->text().length() > 0){
        QMessageBox::warning(this,"��ʾ","����ˣ�����ɾ����");
        return ;
    }
    if(QMessageBox::warning(this,"��ʾ","�Ƿ�ɾ����",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
        SaSettleVouchBean *bean = SaSettleDAO::getSaSettleByNo(mNo);
        bean->setNo(mNo);
        if(bean->isHandle()){
            QMessageBox::warning(this,"��ʾ",bean->handler() + " ����ˣ�����ɾ����");
            return ;
        }
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int mResult = SaSettleDAO::delSaSettleByNo(bean);
        int cResult = SaSettleDAO::delSaSettleVouchsByNo(ui->noEdit->text());
        if(mResult > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            mIsEdit =false;
            setInputEnable(false);
            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
            if(mRecord ==0){
                sNext();
            }else sPrevious();
            mCountRecord = SaSettleDAO::getSaSettleCount("01") - 1;
        }else{
            QMessageBox::critical(this,"����","ɾ������");
            DBManager::getDBManager()->getGoinDBConnection().rollback();
        }
        readAuthority();

    }
}

void SaSettleWin::sAudit(){
    SaSettleVouchBean *bean = SaSettleDAO::getSaSettleByNo(ui->noEdit->text().trimmed());
    QString rdNo = ui->rdNoEdit->text();
    if(bean && bean->no().length() > 0){
        if(bean->isHandle()){
            ui->toolBar->actions().at(4)->setEnabled(false);
            ui->toolBar->actions().at(5)->setEnabled(true);
            QMessageBox::information(this,"��ʾ","����ˣ���������ˣ�");
            return ;
        }

        for(int i = 0; i < mChildVouchsList.size(); i++){
            SaSettleVouchsBean *oBean = SaSettleDAO::getSaSettleRdsBy(mChildVouchsList.at(i)->rdNo(),mChildVouchsList.at(i)->productCode());
            if(oBean && oBean->rdNo().length() > 2){
                double real = oBean->quantity();
                //qDebug() << real << " quantity:" << mAddChildVouchsList.at(i)->quantity();

                if(qAbs(real) - qAbs(mChildVouchsList.at(i)->quantity()) < 0){
                    QMessageBox::warning(this,"��ʾ","���ⵥ:"+ mChildVouchsList.at(i)->rdNo() + " ,��Ʒ��"
                                         + mChildVouchsList.at(i)->productName() + " �ɽ����������㣡,����������Ϊ:" + QString::number(real));
                    return ;
                }

            }else{
                QMessageBox::warning(this,"��ʾ","���ⵥ:"+ mChildVouchsList.at(i)->rdNo() + " ,��Ʒ��"
                                     + mChildVouchsList.at(i)->productName() + " ������" );
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
        int result = SaSettleDAO::updateSaSettle(bean);
        if(result > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            ui->auditEdit->setText(bean->handler());
            if(SaSettleDAO::getSaSettleRdsCount(rdNo) == 0){
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
            QMessageBox::information(this,"��ʾ","��˳ɹ���");
        }else{
            DBManager::getDBManager()->getGoinDBConnection().rollback();
            QMessageBox::information(this,"��ʾ","��˳���");
        }

    }else{
        QMessageBox::critical(this,"����","�޴˵��ݣ��������");
    }
}

void SaSettleWin::sAbAudit(){
    SaSettleVouchBean *bean = SaSettleDAO::getSaSettleByNo(ui->noEdit->text().trimmed());
    if(bean !=NULL && bean->no().length() > 0){
        if(!bean->isHandle()){
            ui->toolBar->actions().at(4)->setEnabled(true);
            ui->toolBar->actions().at(5)->setEnabled(false);
            QMessageBox::information(this,"��ʾ","δ��ˣ�����������");
            return ;
        }

        DBManager::getDBManager()->getGoinDBConnection().transaction();

        bean->setHandler("");
        bean->setIsHandle(false);
        bean->setUpdated(QDateTime::currentDateTime());
        bean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        bean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        int result = SaSettleDAO::updateSaSettle(bean);
        RdBean *rdBean = RdDAO::getRdByNo(ui->rdNoEdit->text());
        if(rdBean && rdBean->rdNo().length()> 2){
            rdBean->setIsBill(false);
            rdBean->setBillNo("");
            result = RdDAO::updateRd(rdBean);
        }
        if(result > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            ui->auditEdit->setText(bean->handler());
            ui->toolBar->actions().at(2)->setEnabled(true);
            ui->toolBar->actions().at(4)->setEnabled(true);
            ui->toolBar->actions().at(5)->setEnabled(false);
            readAuthority();
            QMessageBox::information(this,"��ʾ","����ɹ���");
        }else{
            DBManager::getDBManager()->getGoinDBConnection().rollback();
            QMessageBox::information(this,"��ʾ","�������");
        }

    }
}

void SaSettleWin::sDelRow(){
    if(!mIsEdit){
        QMessageBox::information(this,"��ʾ","�����޸ģ�");
        return;
    }
    QAbstractItemModel *modelTotal = ui->tableWidget->model();
    QItemSelectionModel *seleced = ui->tableWidget->selectionModel();
    int row = seleced->currentIndex().row();

    if(row >= 0){

        QString name = modelTotal->data(modelTotal->index(row,1)).toString();
        QString code = modelTotal->data(modelTotal->index(row,0)).toString();
        if(name.length() < 1){
            QMessageBox::information(this,"��ʾ","����ɾ�����У�");
            return;
        }
        if(QMessageBox::warning(this, tr("Prompt"),"�Ƿ�Ҫɾ���� *" + name + "*",
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
        QMessageBox::information(this,"Prompt","��ѡ��Ҫɾ������!");
    }
}

void SaSettleWin::sPrint(){
    QString fileName = "ReportSaSettle.xml";
    QString title = "���۽��㵥";
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

void SaSettleWin::sChoose(){
    if(ui->cusQks->text().trimmed().length() < 1){
        QMessageBox::warning(this,"��ʾ","������ͻ�");
        return ;
    }
    ChooseVouchDialog *cVouchDlg = new ChooseVouchDialog(this);
    cVouchDlg->setWindowTitle("��ȡ��ͨ���۳��ⵥ");

    childItemProperties childItem[] = {
            { "rd_no","���ⵥ��","���ⵥ��", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "bus_type","��������","��������", 2,80, true, false },
            { "cus_code","�ͻ�����","�ͻ�����", 3,80, true, false },
            { "cus_name","�ͻ�����","�ͻ�����", 4, 120,true, true },
            { "product_code","��Ʒ����","��Ʒ����", 5,80, true, true },
            { "product_name","��Ʒ����","��Ʒ����", 6,100, true, true },
            { "product_style","���","��Ʒ",7,80, true, false },
            { "unit","��λ","��λ", 8,60, true, false },
            { "quantity","��������","��������", 9, 80,true, true },
            { "not_quantity","�ɽ�����","�ɽ�����", 10, 80,true, true },
            { "unit_price","���۵���","���۵���", 11,80, true, true },
            { "money","���۽��","���۽��", 12,80, true, false }

    };


    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    QList<CustomerBean *> cusBeanList = CustomerDAO::queryCus();
    QStringList venList ;
    for(int i =0 ; i < cusBeanList.size(); i++){
        venList << cusBeanList.at(i)->cusCode() + " - " +cusBeanList.at(i)->cusName();
    }
    QDate oneDate = QDate(HaoKeApp::getInstance()->getOperateDate().year(),HaoKeApp::getInstance()->getOperateDate().month(),1);
    cVouchDlg->sDateEdit1->setDate(oneDate);
    cVouchDlg->dwNameQks->setListModel(true);
    cVouchDlg->dwNameQks->setItems(venList);
    CustomerBean *cusBean = CustomerDAO::getCusByCode(ui->cusQks->text());
    cVouchDlg->dwNameQks->setText(cusBean->cusCode() + " - " + cusBean->cusName());
    cVouchDlg->dwNameQks->setReadOnly(true);
    QList<CustomerTypeBean *> typeBeanList = CustomerTypeDAO::queryType();
    QStringList typeList ;
    for(int i =0 ; i < typeBeanList.size(); i++){
        typeList << typeBeanList.at(i)->typeId() + " - " +typeBeanList.at(i)->typeName();
    }
    cVouchDlg->dwTypeQks->setListModel(true);
    cVouchDlg->dwTypeQks->setItems(typeList);
    cVouchDlg->dwTypeQks->setReadOnly(true);
    QString sql = " SELECT a.id, a.rd_no, a.bus_date, a.bus_type, a.cus_code, b.cus_name, a.wh_code,a.dep_code,c.product_code,"
                " d.product_name, c.product_style, c.unit, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
                " a.status, a.memo, c.quantity,c.not_quantity , c.unit_price,c.money "
                " from rd_record a, "
                    " ( SELECT g.*,g.quantity-COALESCE(h.sum_qunatity,0) as not_quantity "
                    " from rd_records g "
                    " LEFT JOIN ( "
                        " SELECT e.rd_no,e.product_code,sum(quantity) as sum_qunatity "
                        " from sa_settle_vouchs e "
                        " GROUP BY e.rd_no,e.product_code) h "
                    " ON g.rd_no=h.rd_no AND g.product_code=h.product_code "
                    " where g.quantity-COALESCE(h.sum_qunatity,0) > 0 "
                " ) c,ba_product d,ba_customer b "
                " WHERE a.bus_date>=? AND a.bus_date<=? AND b.cus_type like ? AND a.cus_code like ? "
                " AND d.type_id like ? AND c.product_code like ?"
                " AND a.rd_no=c.rd_no AND a.is_handle=true AND c.product_code=d.product_code AND a.cus_code= b.cus_code "
                " AND a.vouch_type='02' ORDER BY id ASC";
    if(ui->busTypeComboBox->currentText() == "���ֽ���"){
        sql = " SELECT a.id, a.rd_no, a.bus_date, a.bus_type, a.cus_code, b.cus_name, a.wh_code,a.dep_code,c.product_code,"
                        " d.product_name, c.product_style, c.unit, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
                        " a.status, a.memo, c.quantity,c.not_quantity , c.unit_price,c.money "
                        " from rd_record a, "
                            " ( SELECT g.*,g.quantity-COALESCE(h.sum_qunatity,0) as not_quantity "
                            " from rd_records g "
                            " LEFT JOIN ( "
                                " SELECT e.rd_no,e.product_code,sum(quantity) as sum_qunatity "
                                " from sa_settle_vouchs e "
                                " GROUP BY e.rd_no,e.product_code) h "
                            " ON g.rd_no=h.rd_no AND g.product_code=h.product_code "
                            " where (-g.quantity)-(-COALESCE(h.sum_qunatity,0)) > 0 "
                        " ) c,ba_product d,ba_customer b "
                        " WHERE a.bus_date>=? AND a.bus_date<=? AND b.cus_type like ? AND a.cus_code like ? "
                        " AND d.type_id like ? AND c.product_code like ?"
                        " AND a.rd_no=c.rd_no AND a.is_handle=true AND c.product_code=d.product_code AND a.cus_code= b.cus_code "
                        " AND a.vouch_type='15' ORDER BY id ASC";
    }
    cVouchDlg->initData(dItem,sql);
    cVouchDlg->initTableHead();

    if(cVouchDlg->exec() == QDialog::Accepted){
        QItemSelectionModel *selectionModel = cVouchDlg->tableView->selectionModel();
        QModelIndexList selected= selectionModel->selectedIndexes();
        if(selected.size()<1){
            QMessageBox::information(this,"��ʾ","δ��ȡ����");

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

void SaSettleWin::setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage){
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


void SaSettleWin::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if(item->column() == 0 && mIsEdit){
        QString code = item->data(Qt::EditRole).toString().trimmed();
        if(code.length() < 1){
            return ;
        }
        //�ظ��ж�
        for(int i = 0; i < ui->tableWidget->rowCount(); i ++){
            if(i != item->row() && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,0)).toString().trimmed() == code){
                QMessageBox::information(this,"��ʾ","����Ʒ,���룺" + code + " �����룡");
                ui->tableWidget->model()->setData(ui->tableWidget->model()->index(item->row(),0),"");
                return;
            }
        }

        //����ֱ��¼����Ʒ����
        if(true && !mIsChoose){
            QMessageBox::warning(this,"��ʾ","���㲻��ֱ��¼����Ʒ,����ȡ���ⵥ�ݣ�");
            ui->tableWidget->model()->setData(ui->tableWidget->model()->index(item->row(),0),"");
            return ;
        }
        //����ID��ѯԭ����Ϣ
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

                    //�����޸� ��������
                    SaSettleVouchsBean *puVouchsBean = new SaSettleVouchsBean();
                    puVouchsBean->setSettleNo(ui->noEdit->text());
                    WidgetBeanUtil::setItemModelToBean(model,item->row(),mChildItem,puVouchsBean);
                    puVouchsBean->setTaxPrice(puVouchsBean->sPrice());
                    mAddChildVouchsList.append(puVouchsBean);
                }
                ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            }
        }else{
            QMessageBox::information(this,"��ʾ","�����ڴ���Ʒ");

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

    //�����޸�
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
        //�����޸� ��������
        for(int i =0; i < mChildVouchsList.size(); i++){
            if(mChildVouchsList.at(i)->id() == id){
                double money = price * quantity;
                WidgetBeanUtil::setItemModelToBean(model,item->row(),mChildItem,mChildVouchsList.at(i));

                //mChildVouchsList.at(i)->setMoney(money);

            }
        }

    }
}


