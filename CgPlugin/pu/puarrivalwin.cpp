#include "puarrivalwin.h"
#include "ui_puarrivalwin.h"
#include "haokeapp.h"
#include "bean/DepBean.h"
#include "bean/SupplieBean.h"
#include "bean/EmployeeBean.h"
#include "bean/WareHouseBean.h"
#include "bean/ProductBean.h"
#include "bean/SystemSetBean.h"
#include "bean/RdBean.h"
#include "bean/RdsBean.h"
#include "bean/DefineBean.h"
#include "dao/commondao.h"
#include "dao/depdao.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "dao/employeedao.h"
#include "dao/waredao.h"
#include "dao/productdao.h"
#include "dao/ArrivalDAO.h"
#include "dao/arrivaldao.h"
#include "dao/rddao.h"
#include "dao/systemsetdao.h"
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

PuArrivalWin::PuArrivalWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PuArrivalWin)
{
    ui->setupUi(this);
    init();
    //qApp->setStyle("Plastique");
    setupContextMenu();
    mCountRecord = ArrivalDAO::getPuArrivalCount("01") - 1;
    mRecord = mCountRecord;
    if(mCountRecord < 0) mCountRecord = 0;
    QList<PuArrivalVouchBean *> vouchBeanList = ArrivalDAO::queryPuArrivalByPage(mCountRecord,1,"01");
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

PuArrivalWin::PuArrivalWin(QString no, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::PuArrivalWin)
{
    ui->setupUi(this);
    init();
    setupContextMenu();
    mCountRecord = ArrivalDAO::getPuArrivalCount("01") - 1;
    if(mCountRecord < 0) mCountRecord = 0;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(7)->setEnabled(true);

    PuArrivalVouchBean *bean = ArrivalDAO::getPuArrivalByNo(no);
    if(bean != NULL && bean->no().length() > 0){
        mRecord = ArrivalDAO::getPuArrivalRowNumber(bean->id()) - 1;
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
PuArrivalWin::~PuArrivalWin()
{
    delete ui;
}

void PuArrivalWin::readAuthority(){
    LimitsCommon::authority(ui->toolBar,mWinId.toInt());
}
void PuArrivalWin::init(){
    //����ID��
    mWinId = HaoKeApp::puSubId() + CgPlugin::arrivalWinId();
    mWinTitle = "�ɹ�������";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::puSubId().toInt(),mWinId.toInt(),0,"");

    //���ܲ˵�
    actionProperties actsBase[]= {
        { "puArrival.new",tr("&New"),SLOT(sNew()), NULL, "New", QPixmap(":/image/Add.png"), NULL,  true, tr("New") },
        { "puArrival.edit",tr("&Edit"),SLOT(sEdit()), NULL, "Edit", QPixmap(":/image/edit.png"), NULL,  true, tr("Edit") },
        { "puArrival.delete",tr("&Delete"),SLOT(sDelete()), NULL, "Delete", QPixmap(":/image/cross_48.png"), NULL,  true, tr("Delete") },
        { "puArrival.save",tr("&Save"),SLOT(sSave()), NULL, "Save", QPixmap(":/image/save.png"), NULL,  true, tr("Save") },
        { "puArrival.audit",tr("&Audit"),SLOT(sAudit()), NULL, "Audit", QPixmap(":/image/stamp_48.png"), NULL,  true, tr("Audit") },
        { "puArrival.abandonAudit",tr("&Abandon Audit"),SLOT(sAbAudit()), NULL, "Abandon Audit", QPixmap(":/image/c_stamp_48.png"), NULL,  true, tr("Abandon Audit") },
        { "puArrival.delRow",tr("&DelRow"),SLOT(sDelRow()), NULL, "DelRow", QPixmap(":/image/minus.png"), NULL,  true, tr("DelRow") },
        { "puArrival.print",tr("&Print"),SLOT(sPrint()), NULL, "Print", QPixmap(":/image/printer.png"), NULL,  true, tr("Print") },
        { "puArrival.choose",tr("&Choose"),SLOT(sChoose()), NULL, "Choose", QPixmap(":/image/page_accept.png"), NULL,  true, tr("Choose") },
        { "puArrival.first",tr("&First"),SLOT(sFirst()), NULL, "First", QPixmap(":/image/first48.png"), NULL,  true, tr("First") },
        { "puArrival.next",tr("&Next"),SLOT(sNext()), NULL, "Next", QPixmap(":/image/n-next48.png"), NULL,  true, tr("Next") },
        { "puArrival.previous",tr("&Previous"),SLOT(sPrevious()), NULL, "Previous", QPixmap(":/image/n-previous48.png"), NULL,  true, tr("Previous") },
        { "puArrival.end",tr("&End"),SLOT(sEnd()), NULL, "End", QPixmap(":/image/end48.png"), NULL,  true, tr("End") },
        { "puArrival.cancel",tr("&Cancel"),SLOT(sCancel()), NULL, "Cancel", QPixmap(":/image/cancel.png"), NULL,  true, tr("Cancel") }

    };
    addActionsTo(actsBase,sizeof(actsBase) / sizeof(actsBase[0]));

    mIsEdit =false;

    ui->dateEdit->setDate(HaoKeApp::getInstance()->getOperateDate());
    mIsChoose = false;

    mIsHathOrder = false; //�ɹ�ҵ����ж���
    mIsMakeRd = false; //����Ƿ�������ⵥ
    SystemSetBean *hathOrderBean = SystemSetDAO::getBean("001");
    if(hathOrderBean && hathOrderBean->id().length() > 0){
        mIsHathOrder = hathOrderBean->yesNo();
    }

    SystemSetBean *makeRdBean = SystemSetDAO::getBean("002");
    if(makeRdBean && makeRdBean->id().length() > 0){
        mIsMakeRd = makeRdBean->yesNo();
    }
    //��ʼ�������ʽ
    setInputEnable(false);

    childItemProperties childItem[] = {
            { "productCode","��Ʒ����","��Ʒ����", 0,180, true, true },
            { "productName","��Ʒ����","��Ʒ����", 1,180, true, true },
            { "productStyle","��Ʒ���","��Ʒ���", 2,80, true, false },
            { "unit","�ɹ���λ","�ɹ���λ", 3,80, true, false },
            { "quantity","����","����", 4, 60,true, true },
            { "price","�ɹ�����","�ɹ�����", 5,80, true, true },
            { "money","�ɹ����","�ɹ����", 6,80, true, false },
            { "id","ID","ID", 7,60, false, false },
            { "orderNo","������","������", 8, 120,true, false },
            { "define1","�Զ���1","�Զ���1", 9,100, false, true },
                    { "define2","�Զ���2","�Զ���2", 10,100, false, true },
                    { "define3","�Զ���3","�Զ���3", 11,100, false, true },
                    { "define4","�Զ���4","�Զ���4", 12,100, false, true },
                    { "define5","�Զ���5","�Զ���5", 13,100, false, true },
                    { "define6","�Զ���6","�Զ���6", 14,100, false, true },
                    { "memo","��ע","��ע", 15,160, false, true }
    };
    int size = sizeof(childItem) / sizeof(childItem[0]);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(size);
    //qDebug() << size;
    for(int i =0 ; i < size ; i ++){
        mChildItem << childItem[i];
    }

    //�Ƿ�ʹ���Զ����ֶ�
    SystemSetBean *useDefineBean = SystemSetDAO::getBean("101");
    if(useDefineBean && useDefineBean->id().length() > 0){
        if(useDefineBean->yesNo()){
            QList<DefineBean *> defineList = CommonDAO::queryDefine();
            for(int i =9 ; i < size ; i ++){
                for(int j =0 ; j < defineList.size() ; j ++){
                    if(mChildItem.at(i).name.trimmed() ==   defineList.at(j)->code()){
                        mChildItem[i].cnName = defineList.at(j)->name();
                        mChildItem[i].title = defineList.at(j)->name();
                        mChildItem[i].visible = defineList.at(j)->isView();

                    }
                }
            }
        }
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
    busTypeList << "��ͨ�ɹ�";
    busTypeList << "�ƻ��ɹ�" << "��ͬ�ɹ�" <<"�������ɹ�";
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

    QList<WareHouseBean *> whBeanList = WareDAO::queryWarehouse();
    QStringList whList ;
    for(int i =0 ; i < whBeanList.size(); i++){
        whList << whBeanList.at(i)->whCode() + " - " +whBeanList.at(i)->whName();
    }
    ui->whSearchEditbox->setListModel(true);
    ui->whSearchEditbox->setItems(whList);

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

void PuArrivalWin::sInvClick(){

    QString sql = " SELECT a.* FROM ba_product a"
            " WHERE a.product_code LIKE ? AND a.type_id LIKE ? ";
    childItemProperties childItem[] = {
            { "product_code","������Ʒ","��Ʒ", 0,60, true, true },
            { "product_name","������Ʒ","��Ʒ", 1,100, true, true },
            { "style","���","���", 2,80, true, true },
            { "purchase_unit","��λ","��λ", 3, 60,true, true },
            { "pu_price","�۸�","�۸�", 4, 60,true, true }
    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    QString whCode = ui->whSearchEditbox->text() + " - " + ui->whSearchEditbox->subText();
    QString invCode = "";
    OptInvDialog *invDlg = new OptInvDialog(dItem,sql,invCode,this);

    if(invDlg->exec() == QDialog::Accepted){
        QItemSelectionModel *selectionModel = invDlg->getTableView()->selectionModel();
        QModelIndexList selected= selectionModel->selectedIndexes();
        if(selected.size() < 1){
            QMessageBox::information(this,"��ʾ","ĩѡ����Ʒ!");

        }else{
            invCode = invDlg->getTableView()->model()->data(invDlg->getTableView()->model()->index(selected.at(0).row(),0)).toString();
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,ui->tableWidget->currentRow(),mChildItem,"productCode",invCode);
        }
    }
    delete invDlg;

}
void PuArrivalWin::footResized(int logicalIndex, int oldSize, int newSize){
    mFootView->resizeSection(logicalIndex + 1,newSize);
}
void PuArrivalWin::refreshData(){

    QStringList headerLabels;
    for (int i = 0; i < mChildItem.size(); i++){
        headerLabels.append(mChildItem[i].cnName);
        ui->tableWidget->setColumnWidth(mChildItem[i].id,mChildItem[i].width);
        mFootView->resizeSection(mChildItem[i].id + 1,mChildItem[i].width);
        if(!mChildItem[i].visible)ui->tableWidget->hideColumn(mChildItem[i].id);
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
}

void PuArrivalWin::read(QString no){
    PuArrivalVouchBean *bean = ArrivalDAO::getPuArrivalByNo(no);
    mNo = no;
    if(bean != NULL && bean->no().length() > 0){
        ui->noEdit->setText(bean->no());
        ui->dateEdit->setDate(bean->busDate());
        SupplieBean *venBean = SupplieDAO::getSupplieByCode(bean->supplierId());
        ui->venSearchEditbox->setText(bean->supplierId() + " - " + venBean->supplieName());
        WareHouseBean *whBean = WareDAO::getWarehouseById(bean->whCode());
        ui->whSearchEditbox->setText(bean->whCode() + " - " + whBean->whName());;
        DepBean *depBean = DepDAO::getDepById(bean->depCode());
        ui->depSearchEditbox->setText(bean->depCode() + " - " + depBean->depName());
        ui->userSearchEditbox->setText(bean->personCode() + " - " + CacheBaBeanCommon::personToName(bean->personCode()));
        ui->memoEdit->setText(bean->memo());
        ui->makerEdit->setText(CacheBaBeanCommon::userToName(bean->createdBy(),1));
        ui->auditEdit->setText(bean->handler());
        for(int i = 0; i < ui->busTypeComboBox->count(); i++){
            if(bean->busType() == ui->busTypeComboBox->itemText(i)){
                ui->busTypeComboBox->setCurrentIndex(i);
            }
        }
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
        ui->toolBar->actions().at(3)->setEnabled(false);
        ui->toolBar->actions().at(6)->setEnabled(false);
        ui->toolBar->actions().at(8)->setEnabled(false);

        if(bean->handler().length() > 1){
            ui->toolBar->actions().at(4)->setEnabled(false);
            ui->toolBar->actions().at(5)->setEnabled(true);
        }else{
            ui->toolBar->actions().at(4)->setEnabled(true);
            ui->toolBar->actions().at(5)->setEnabled(false);
        }
        QList<PuArrivalVouchsBean *> pBeanList = ArrivalDAO::queryPuArrivalVouchsByNo(no);
        mChildVouchsList = ArrivalDAO::queryPuArrivalVouchsByNo(no);
        ui->tableWidget->setRowCount(pBeanList.size() + 1);
        mTotalQuantity = 0;
        mTotalSum = 0;
        for(int i =0; i < pBeanList.size(); i++){
            WidgetBeanUtil::setBeanToTableWidget(ui->tableWidget,i,mChildItem,pBeanList.at(i));
            mTotalQuantity = mTotalQuantity + pBeanList.at(i)->quantity();
            mTotalSum = mTotalSum + pBeanList.at(i)->money();
        }

        mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity,'f',2)));
        mFootModel->setItem(7,new QStandardItem(QString::number(mTotalSum,'f',2)));
        mFootView->reset();
        readAuthority();
    }
}

void PuArrivalWin::inputClear(){
    WidgetBeanUtil::clearLayoutWidgetData(ui->headGridLayout);


}

void PuArrivalWin::setInputEnable(bool b){
     WidgetBeanUtil::setLayoutWidgetEnable(ui->headGridLayout,b);
     if(b){
         ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);
     }else{
         ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
     }
}

void PuArrivalWin::addActionsTo(actionProperties acts[], unsigned int numElems){
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

void PuArrivalWin::toolBarActTriggered(QAction *act){
    //д�������־
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::puSubId().toInt(),mWinId.toInt(),act->objectName());

}
void PuArrivalWin::sFirst(){
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
    QList<PuArrivalVouchBean *> vouchBeanList = ArrivalDAO::queryPuArrivalByPage(0,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());

    }
    baseToolBar->actions().at(0)->setEnabled(false);
    baseToolBar->actions().at(1)->setEnabled(true);
    baseToolBar->actions().at(2)->setEnabled(false);
    baseToolBar->actions().at(3)->setEnabled(true);
}

void PuArrivalWin::sNext(){
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
    QList<PuArrivalVouchBean *> vouchBeanList = ArrivalDAO::queryPuArrivalByPage(mRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(2)->setEnabled(true);
    baseToolBar->actions().at(0)->setEnabled(true);
}

void PuArrivalWin::sPrevious(){
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
    QList<PuArrivalVouchBean *> vouchBeanList = ArrivalDAO::queryPuArrivalByPage(mRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(1)->setEnabled(true);
    baseToolBar->actions().at(3)->setEnabled(true);
}

void PuArrivalWin::sEnd(){
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
    mCountRecord = ArrivalDAO::getPuArrivalCount("01") - 1;
    mRecord = mCountRecord;
    QList<PuArrivalVouchBean *> vouchBeanList = ArrivalDAO::queryPuArrivalByPage(mCountRecord,1,"01");
    if(vouchBeanList.size()>0){
        read(vouchBeanList.at(0)->no());
    }
    baseToolBar->actions().at(0)->setEnabled(true);
    baseToolBar->actions().at(1)->setEnabled(false);
    baseToolBar->actions().at(2)->setEnabled(true);
    baseToolBar->actions().at(3)->setEnabled(false);
}

void PuArrivalWin::sCancel(){

    this->close();
}

void PuArrivalWin::closeEvent(QCloseEvent *event){

    if(mIsEdit){
        if(QMessageBox::warning(this,"��ʾ","�Ƿ��������",QMessageBox::Yes | QMessageBox::No)==QMessageBox::No){
            event->ignore();
            return ;
        }
    }

    event->accept();
    HaoKeApp::getInstance()->gui()->removeSubWin(this);


}

void PuArrivalWin::setupContextMenu(){
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void PuArrivalWin::sNew(){
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
    mNo = PuItemUtil::getToDayMaxArrivalId(QDate::currentDate());
    ui->noEdit->setText(mNo);
    refreshData();

    mTotalQuantity = 0;
    mTotalSum = 0;

    mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity)));
    mFootModel->setItem(7,new QStandardItem(QString::number(mTotalSum)));
    mFootView->reset();
    readAuthority();
}

void PuArrivalWin::sEdit(){
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

void PuArrivalWin::sSave(){
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

    if(ui->venSearchEditbox->text().trimmed().length() < 1){
        QMessageBox::warning(this,"��ʾ","�����빩Ӧ��");
        return ;
    }
    if(ui->whSearchEditbox->text().trimmed().length() < 1){
        QMessageBox::warning(this,"��ʾ","������ֿ�");
        return ;
    }
    for(int i = 0;i < rowNum; i++){
        QString pCode = model->data(model->index(i,0)).toString();
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
            QMessageBox::warning(this,"��ʾ","��ԭ��Ʒ���룬�����ڣ����ܱ���");
            return ;
        }
    }
    if(mEditStatus == addFlag){
        if(ArrivalDAO::getPuArrivalCountByNo(ui->noEdit->text()) > 0){
            QMessageBox::warning(this,"��ʾ","�����˵����ظ������ܱ��棬���������µ���");
            QString no = PuItemUtil::getToDayMaxArrivalId(QDate::currentDate());
            ui->noEdit->setText(no);
            return ;
        }

        mNo = ui->noEdit->text();
        if(mIsHathOrder){
            for(int i = 0;i < rowNum; i++){
                PuArrivalVouchsBean *puVouchsBean = new PuArrivalVouchsBean();
                puVouchsBean->setArrivalNo(mNo);
                WidgetBeanUtil::setItemModelToBean(model,i,mChildItem,puVouchsBean);
                PuOrderVouchsBean *oBean = OrderDAO::getPuOrderVouchsBy(puVouchsBean->orderNo(),puVouchsBean->productCode());
                if(oBean && oBean->orderNo().length() > 2){
                    double real = oBean->realQuantity();
                    qDebug() << real << " quantity:" << puVouchsBean->quantity();
                    if(real - puVouchsBean->quantity() < 0){
                        QMessageBox::warning(this,"��ʾ","����:"+ puVouchsBean->orderNo() + " ,��Ʒ��"
                                             + puVouchsBean->productName() + " �ɵ����������㣡,��󵽻�����Ϊ:" + QString::number(real));
                        return ;
                    }

                }else{
                    QMessageBox::warning(this,"��ʾ","����:"+ puVouchsBean->orderNo() + " ,��Ʒ��"
                                         + puVouchsBean->productName() + " ������" );
                    return ;
                }
            }
        }
        PuArrivalVouchBean *puVouchBean = new PuArrivalVouchBean();
        puVouchBean->setNo(ui->noEdit->text());
        QDateTime time = QDateTime::currentDateTime();
        puVouchBean->setBusDate(ui->dateEdit->date());
        puVouchBean->setBusType("01");
        puVouchBean->setPuType(ui->busTypeComboBox->currentText());
        puVouchBean->setSupplierId(ui->venSearchEditbox->text());
        puVouchBean->setDepCode(ui->depSearchEditbox->text());
        puVouchBean->setPersonCode(ui->userSearchEditbox->text());
        puVouchBean->setWhCode(ui->whSearchEditbox->text());
        puVouchBean->setMemo(ui->memoEdit->text());
        puVouchBean->setHandler("");
        puVouchBean->setCreated(time);
        puVouchBean->setIsHandle(false);
        puVouchBean->setIsComplete(false);
        puVouchBean->setCreatedBy(HaoKeApp::getInstance()->getUserId());
        puVouchBean->setCreater(HaoKeApp::getInstance()->getUserDesc());
        puVouchBean->setUpdated(time);
        puVouchBean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        puVouchBean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        puVouchBean->setStatus(0);
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int mResult = 0;
        mResult = ArrivalDAO::createPuArrival(puVouchBean);
        int cResult = 0;

        for(int i = 0;i < rowNum; i++){
            PuArrivalVouchsBean *puVouchsBean = new PuArrivalVouchsBean();
            puVouchsBean->setArrivalNo(mNo);
            WidgetBeanUtil::setItemModelToBean(model,i,mChildItem,puVouchsBean);

            puVouchsBean->setTaxPrice(puVouchsBean->price());

            int c = ArrivalDAO::createPuArrivalVouchs(puVouchsBean);
            if(c < 1){
                DBManager::getDBManager()->getGoinDBConnection().rollback();
                QMessageBox::information(this,"��ʾ","��" + puVouchsBean->productCode() + " �����ύ����,���ܱ���");
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
            mCountRecord = ArrivalDAO::getPuArrivalCount("01") - 1;
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
        PuArrivalVouchBean *puVouchBean = ArrivalDAO::getPuArrivalByNo(mNo);
        if(puVouchBean && puVouchBean->isHandle()){
            QMessageBox::warning(this,"��ʾ","�˵�����ˣ������޸�");
            return ;
        }
        if(mIsHathOrder){
            for(int i = 0; i < mChildVouchsList.size(); i++){
                PuOrderVouchsBean *oBean = OrderDAO::getPuOrderVouchsBy(mChildVouchsList.at(i)->orderNo(),mChildVouchsList.at(i)->productCode());
                if(oBean && oBean->orderNo().length() > 2){
                    double real = oBean->realQuantity();
                    //qDebug() << real << " quantity:" << mAddChildVouchsList.at(i)->quantity();
                    PuArrivalVouchsBean *aBean = ArrivalDAO::getPuArrivalBy(mChildVouchsList.at(i)->arrivalNo(),mChildVouchsList.at(i)->productCode());
                    if(aBean && aBean->orderNo() == oBean->orderNo()){
                        real = real + aBean->quantity();
                    }
                    if(real - mChildVouchsList.at(i)->quantity() < 0){
                        QMessageBox::warning(this,"��ʾ","����:"+ mChildVouchsList.at(i)->orderNo() + " ,��Ʒ��"
                                             + mChildVouchsList.at(i)->productName() + " �ɵ����������㣡,��󵽻�����Ϊ:" + QString::number(real));
                        return ;
                    }

                }else{
                    QMessageBox::warning(this,"��ʾ","����:"+ mChildVouchsList.at(i)->orderNo() + " ,��Ʒ��"
                                         + mChildVouchsList.at(i)->productName() + " ������");
                    return ;
                }
            }

            for(int i = 0; i < mAddChildVouchsList.size(); i++){
                PuOrderVouchsBean *oBean = OrderDAO::getPuOrderVouchsBy(mAddChildVouchsList.at(i)->orderNo(),mAddChildVouchsList.at(i)->productCode());
                if(oBean && oBean->orderNo().length() > 2){
                    double real = oBean->realQuantity();
                    //qDebug() << real << " quantity:" << mAddChildVouchsList.at(i)->quantity();
                    PuArrivalVouchsBean *aBean = ArrivalDAO::getPuArrivalBy(mAddChildVouchsList.at(i)->arrivalNo(),mAddChildVouchsList.at(i)->productCode());
                    if(aBean && aBean->orderNo() == oBean->orderNo()){
                        real = real + aBean->quantity();
                    }
                    if(real - mAddChildVouchsList.at(i)->quantity() < 0){
                        QMessageBox::warning(this,"��ʾ","����:"+ mAddChildVouchsList.at(i)->orderNo() + " ,��Ʒ��"
                                             + mAddChildVouchsList.at(i)->productName() + " �ɵ����������㣡,��󵽻�����Ϊ:" + QString::number(real));
                        return ;
                    }

                }else{
                    QMessageBox::warning(this,"��ʾ","����:"+ mAddChildVouchsList.at(i)->orderNo() + " ,��Ʒ��"
                                         + mAddChildVouchsList.at(i)->productName() + " ������" );
                    return ;
                }

            }
        }
        puVouchBean->setNo(ui->noEdit->text());
        QDateTime time = QDateTime::currentDateTime();
        puVouchBean->setBusDate(ui->dateEdit->date());
        puVouchBean->setPuType(ui->busTypeComboBox->currentText());
        puVouchBean->setSupplierId(ui->venSearchEditbox->text());
        puVouchBean->setDepCode(ui->depSearchEditbox->text());
        puVouchBean->setPersonCode(ui->userSearchEditbox->text());
        puVouchBean->setWhCode(ui->whSearchEditbox->text());
        puVouchBean->setMemo(ui->memoEdit->text());
        puVouchBean->setUpdated(time);
        puVouchBean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        puVouchBean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int mResult = 0;
        mResult = ArrivalDAO::updatePuArrival(puVouchBean);
        int cResult = 0;
        //ɾ��
        for(int i = 0; i < mDelChildList.size(); i++){
            PuArrivalVouchsBean *delBean = new PuArrivalVouchsBean;
            delBean->setId(mDelChildList.at(i));
            int c = ArrivalDAO::delPuArrivalVouchs(delBean);
            cResult = cResult + c;
        }
        //����
        for(int i = 0; i < mChildVouchsList.size(); i++){
            int c = ArrivalDAO::updatePuArrivalVouchs(mChildVouchsList.at(i));
            cResult = cResult + c;
        }
        //���
        for(int i = 0; i < mAddChildVouchsList.size(); i++){
            int c = ArrivalDAO::createPuArrivalVouchs(mAddChildVouchsList.at(i));
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

void PuArrivalWin::sDelete(){
    if(ui->auditEdit->text().length() > 0){
        QMessageBox::warning(this,"��ʾ","����ˣ�����ɾ����");
        return ;
    }
    if(QMessageBox::warning(this,"��ʾ","�Ƿ�ɾ����",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
        PuArrivalVouchBean *bean = ArrivalDAO::getPuArrivalByNo(mNo);
        bean->setNo(mNo);
        if(bean->isHandle()){
            QMessageBox::warning(this,"��ʾ",bean->handler() + " ����ˣ�����ɾ����");
            return ;
        }
        RdBean *rdbean = RdDAO::getRdByBusNo(ui->noEdit->text(),HaoKeApp::getVouchRdPuIn());
        if(rdbean && rdbean->rdNo().length() > 0){
            QMessageBox::warning(this,"��ʾ",rdbean->rdNo() + " ��ⵥ�����ɣ��˵�����ɾ����");
            return ;
        }
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int mResult = ArrivalDAO::delPuArrivalByNo(bean);
        int cResult = ArrivalDAO::delPuArrivalVouchsByNo(ui->noEdit->text());
        if(mResult > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            mIsEdit =false;
            setInputEnable(false);
            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
            if(mRecord ==0){
                sNext();
            }else sPrevious();
            mCountRecord = ArrivalDAO::getPuArrivalCount("01") - 1;
        }else{
            DBManager::getDBManager()->getGoinDBConnection().rollback();
            QMessageBox::critical(this,"����","ɾ������");

        }

    }
    readAuthority();
}

void PuArrivalWin::sAudit(){
    PuArrivalVouchBean *bean = ArrivalDAO::getPuArrivalByNo(ui->noEdit->text().trimmed());
    if(bean && bean->no().length() > 0){
        if(bean->isHandle()){
            ui->toolBar->actions().at(4)->setEnabled(false);
            ui->toolBar->actions().at(5)->setEnabled(true);
            QMessageBox::information(this,"��ʾ","����ˣ���������ˣ�");
            return ;
        }
        QList<PuArrivalVouchsBean *> vouchsList = ArrivalDAO::queryPuArrivalVouchsByNo(bean->no());
        //�ж϶����ɵ�������
        if(mIsHathOrder){
            for(int i=0;i<vouchsList.size();i++){
                PuOrderVouchsBean *oBean = OrderDAO::getPuOrderVouchsBy(vouchsList.at(i)->orderNo(),vouchsList.at(i)->productCode());
                double real = 0;
                if(oBean && oBean->orderNo().length() > 2){
                    real = oBean->quantity() - oBean->inQuantity();
                    if(real - vouchsList.at(i)->quantity() < 0){
                        QMessageBox::critical(this,"����","����:" + vouchsList.at(i)->orderNo() + " ��Ʒ��" + vouchsList.at(i)->productName() + " �ɵ�������Ϊ��" + QString::number(real) );
                        return ;
                    }
                }else{
                    QMessageBox::critical(this,"����","����:" + vouchsList.at(i)->orderNo() + " ��Ʒ��" +
                                          vouchsList.at(i)->productName() + " ������");
                    return ;
                }

            }
        }
        if(mIsMakeRd){
            RdBean *rdbean = RdDAO::getRdByBusNo(ui->noEdit->text(),HaoKeApp::getVouchRdPuIn());
            if(rdbean && rdbean->rdNo().length() > 0 ){
                QMessageBox::warning(this,"����",rdbean->rdNo() + " ��ⵥ�Ѵ���,�����ظ�����,�˵���������ˣ�");
                return ;
            }
        }
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int sResult = 0;
        //�޸Ķ�����������
        if(mIsHathOrder){
            for(int i=0;i<vouchsList.size();i++){
                PuOrderVouchsBean *oBean = OrderDAO::getPuOrderVouchsBy(vouchsList.at(i)->orderNo(),vouchsList.at(i)->productCode());
                if(oBean && oBean->orderNo().length() > 2 ){
                    oBean->setInQuantity(oBean->inQuantity() + vouchsList.at(i)->quantity());
                    if(oBean->quantity() == oBean->inQuantity())oBean->setIsComplete(true);
                    OrderDAO::updatePuOrderVouchs(oBean);
                    sResult = sResult + 1;
                }else{
                    DBManager::getDBManager()->getGoinDBConnection().rollback();

                    QMessageBox::critical(this,"����","�޴˶����������,������:" + vouchsList.at(i)->orderNo()  );
                    return ;
                }
            }
        }
        //���������ⵥ
        if(mIsMakeRd){
            int nCount = RdDAO::getRdCount(QDate::currentDate()) ;
            QString rdNo = PuItemUtil::getToDayMaxNo("4004","rd_record",QDate::currentDate(),nCount);
            RdBean *rdbean = new RdBean();
            rdbean->setRdNo(rdNo);
            rdbean->setRdFlag(RdFlag::RdInFlag);
            rdbean->setBusDate(HaoKeApp::getInstance()->getOperateDate());
            rdbean->setBusType(bean->puType());
            rdbean->setSupplierId(bean->supplierId());
            rdbean->setWhCode(bean->whCode());
            rdbean->setDepCode(bean->depCode());
            rdbean->setBusNo(bean->no());
            //bean->setOrderNo(ui->oNoEdit->text());
            rdbean->setVouchType(HaoKeApp::getVouchRdPuIn());
            rdbean->setVouchSource(HaoKeApp::getVouchPuArrival());
            rdbean->setPersonCode(bean->personCode());
            QDateTime time = QDateTime::currentDateTime();
            rdbean->setCreated(time);
            rdbean->setCreatedBy(HaoKeApp::getInstance()->getUserId());
            rdbean->setCreater(HaoKeApp::getInstance()->getUserDesc());
            rdbean->setHandler("");
            rdbean->setIsHandle(false);
            rdbean->setStatus(0);
            rdbean->setImeiAudit(false);
            rdbean->setIsBill(false);
            if(ui->memoEdit->text().length() < 1)
                rdbean->setMemo("�������Զ�����,����:" + bean->no());
            else
                rdbean->setMemo(ui->memoEdit->text());
            int mResult = 0;
            mResult = RdDAO::createRd(rdbean);
            if(mResult > 0){
                for(int i = 0; i < vouchsList.size() ; i++){
                    RdsBean *rdsBean = new RdsBean();
                    rdsBean->setRdNo(rdNo);
                    rdsBean->setProductCode(vouchsList.at(i)->productCode());
                    rdsBean->setProductName(vouchsList.at(i)->productName());
                    rdsBean->setProductStyle(vouchsList.at(i)->productStyle());
                    rdsBean->setUnit(vouchsList.at(i)->unit());
                    rdsBean->setMoney(vouchsList.at(i)->money());
                    rdsBean->setUnitPrice(vouchsList.at(i)->price());
                    rdsBean->setQuantity(vouchsList.at(i)->quantity());
                    rdsBean->setOrderNo(vouchsList.at(i)->orderNo());
                    rdsBean->setBusNo(vouchsList.at(i)->arrivalNo());
                    rdsBean->setMemo(vouchsList.at(i)->memo());
                    rdsBean->setDefine1(vouchsList.at(i)->define1());
                    rdsBean->setDefine2(vouchsList.at(i)->define2());
                    rdsBean->setDefine3(vouchsList.at(i)->define3());
                    rdsBean->setDefine4(vouchsList.at(i)->define4());
                    rdsBean->setDefine5(vouchsList.at(i)->define5());
                    rdsBean->setDefine6(vouchsList.at(i)->define6());
                    rdsBean->setAMoney(0);
                    int c = RdDAO::createRds(rdsBean);
                    if(c < 1){
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        QMessageBox::information(this,"��ʾ","��" + rdsBean->productCode() + " �����ύ����,���ܱ���");
                        return ;
                    }
                    mResult = mResult + c;
                }
            }else{
                DBManager::getDBManager()->getGoinDBConnection().rollback();
                QMessageBox::critical(this,"��ʾ","�Զ�������ⵥ����,���ܱ���");
                return;
            }

        }
        bean->setHandler(CacheBaBeanCommon::userToName(HaoKeApp::getInstance()->getUserId(),1));
        bean->setAuditTime(QDateTime::currentDateTime());
        bean->setIsHandle(true);
        bean->setUpdated(QDateTime::currentDateTime());
        bean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        bean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        int result = ArrivalDAO::updatePuArrival(bean);
        if(mIsHathOrder){
            if(result > 0 && sResult >= vouchsList.size() && DBManager::getDBManager()->getGoinDBConnection().commit()){
                ui->auditEdit->setText(bean->handler());
                ui->toolBar->actions().at(4)->setEnabled(false);
                ui->toolBar->actions().at(5)->setEnabled(true);
                readAuthority();
                QMessageBox::information(this,"��ʾ","��˳ɹ���");
            }else{
                DBManager::getDBManager()->getGoinDBConnection().rollback();
                QMessageBox::information(this,"��ʾ","��˳���");
            }
        }else{
            if(result > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
                ui->auditEdit->setText(bean->handler());
                ui->toolBar->actions().at(4)->setEnabled(false);
                ui->toolBar->actions().at(5)->setEnabled(true);
                readAuthority();
                QMessageBox::information(this,"��ʾ","��˳ɹ���");
            }else{
                DBManager::getDBManager()->getGoinDBConnection().rollback();
                QMessageBox::information(this,"��ʾ","��˳���");
            }

        }

    }else{
        QMessageBox::critical(this,"����","�޴˵��ݣ��������");
    }

}

void PuArrivalWin::sAbAudit(){
    PuArrivalVouchBean *bean = ArrivalDAO::getPuArrivalByNo(ui->noEdit->text().trimmed());
    if(bean !=NULL && bean->no().length() > 0){
        if(!bean->isHandle()){
            ui->toolBar->actions().at(4)->setEnabled(true);
            ui->toolBar->actions().at(5)->setEnabled(false);
            QMessageBox::information(this,"��ʾ","δ��ˣ�����������");
            return ;
        }
        QList<PuArrivalVouchsBean *> vouchsList = ArrivalDAO::queryPuArrivalVouchsByNo(bean->no());
        if(mIsHathOrder){
            for(int i=0;i<vouchsList.size();i++){
                PuOrderVouchsBean *oBean = OrderDAO::getPuOrderVouchsBy(vouchsList.at(i)->orderNo(),vouchsList.at(i)->productCode());
                double real = 0;
                if(oBean && oBean->orderNo().length() > 2){
                    real = oBean->inQuantity();
                    if(real - vouchsList.at(i)->quantity() < 0){
                        QMessageBox::critical(this,"����","����:" + vouchsList.at(i)->orderNo() + " ��Ʒ��"
                                              + vouchsList.at(i)->productName() + " �ɵ�������Ϊ��" + QString::number(real) );
                        return ;
                    }
                }else{
                    QMessageBox::critical(this,"����","����:" + vouchsList.at(i)->orderNo() + " ��Ʒ��"
                                          + vouchsList.at(i)->productName()
                                          + " ������");
                    return ;
                }

            }
        }
        //�����ɾ����ⵥ
        if(mIsMakeRd){
            RdBean *rdbean = RdDAO::getRdByBusNo(ui->noEdit->text(),HaoKeApp::getVouchRdPuIn());
            if(rdbean && rdbean->rdNo().length() > 0 && rdbean->isHandle()){
                QMessageBox::warning(this,"��ʾ",rdbean->rdNo() + " ��ⵥ����ˣ��˵���������ˣ�");
                return ;
            }


        }
        DBManager::getDBManager()->getGoinDBConnection().transaction();
        int sResult = 0;
        //�����ɾ����ⵥ
        if(mIsMakeRd){
            RdBean *rdbean = RdDAO::getRdByBusNo(ui->noEdit->text(),"01");
            RdDAO::delRdByNo(rdbean);
            RdDAO::delRdsByNo(rdbean->rdNo());
        }
        if(mIsHathOrder){
            for(int i=0;i<vouchsList.size();i++){
                PuOrderVouchsBean *oBean = OrderDAO::getPuOrderVouchsBy(vouchsList.at(i)->orderNo(),vouchsList.at(i)->productCode());
                oBean->setInQuantity(oBean->inQuantity() - vouchsList.at(i)->quantity());
                oBean->setIsComplete(false);
                OrderDAO::updatePuOrderVouchs(oBean);
                sResult = sResult + 1;
            }
        }
        bean->setHandler("");
        bean->setIsHandle(false);
        bean->setUpdated(QDateTime::currentDateTime());
        bean->setUpdatedBy(HaoKeApp::getInstance()->getUserId());
        bean->setUpdater(HaoKeApp::getInstance()->getUserDesc());
        int result = ArrivalDAO::updatePuArrival(bean);
        if(result > 0 && DBManager::getDBManager()->getGoinDBConnection().commit()){
            ui->auditEdit->setText(bean->handler());
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

void PuArrivalWin::sDelRow(){
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
                    double money = model->data(model->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"money"))).toDouble();

                    mTotalQuantity = mTotalQuantity + quantity;
                    mTotalSum = mTotalSum + money;

                }

                mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity)));
                mFootModel->setItem(7,new QStandardItem(QString::number(mTotalSum)));
                mFootView->reset();

            }


        }
    }else{
        QMessageBox::information(this,"Prompt","��ѡ��Ҫɾ������!");
    }
}

void PuArrivalWin::sPrint(){
    QString fileName = "ReportPuArrival.xml";
    QString formatData = "";
    QString title = mWinTitle;
    QList<ReportFormatBean *> rptList = ReportDAO::queryRptBy(mWinId);

    if(rptList.size() > 0){
        formatData = rptList.at(rptList.size() - 1)->reportFormat();
    }else {
        formatData = PrintUtils::makeWinPrintFormatData(ui->headGridLayout,mChildItem,mWinTitle,fileName);
        ReportFormatBean *rptBean = new ReportFormatBean();
        rptBean->setReportTitle(title);
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

void PuArrivalWin::sChoose(){
    if(ui->venSearchEditbox->text().trimmed().length() < 1){
        QMessageBox::warning(this,"��ʾ","�����빩Ӧ��");
        return ;
    }
    ChooseVouchDialog *cVouchDlg = new ChooseVouchDialog(this);
    cVouchDlg->setWindowTitle("��ȡ�ɹ�����");

    childItemProperties childItem[] = {
            { "no","������","������", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "bus_type","��������","��������", 2,80, true, false },
            { "supplier_id","��Ӧ�̱���","��Ӧ�̱���", 3,80, true, false },
            { "supplie_name","��Ӧ������","��Ӧ������", 4, 120,true, true },
            { "product_code","��Ʒ����","��Ʒ����", 5,80, true, true },
            { "product_name","��Ʒ����","��Ʒ����", 6,100, true, true },
            { "product_style","���","��Ʒ",7,80, true, false },
            { "unit","�ɹ���λ","�ɹ���λ", 8,60, true, false },
            { "quantity","�ܶ�������","�ܶ�������", 9, 80,true, true },
            { "not_quantity","δ��������","δ��������", 10, 80,true, true },
            { "price","�ɹ�����","�ɹ�����", 11,80, true, true },
            { "money","�ɹ����","�ɹ����", 12,80, true, false },
        { "define1","�Զ���1","�Զ���1", 13,100, false, true },
        { "define2","�Զ���2","�Զ���2", 14,100, false, true },
        { "define3","�Զ���3","�Զ���3", 15,100, false, true },
        { "define4","�Զ���4","�Զ���4", 16,100, false, true },
        { "define5","�Զ���5","�Զ���5", 17,100, false, true },
        { "define6","�Զ���6","�Զ���6", 18,100, false, true },
        { "memo","��ע","��ע", 19,160, false, true }


    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    //�Ƿ�ʹ���Զ����ֶ�
    SystemSetBean *useDefineBean = SystemSetDAO::getBean("101");
    if(useDefineBean && useDefineBean->id().length() > 0){
        if(useDefineBean->yesNo()){
            QList<DefineBean *> defineList = CommonDAO::queryDefine();
            for(int i =10 ; i < size ; i ++){
                for(int j =0 ; j < defineList.size() ; j ++){
                    if(dItem.at(i).name.trimmed() ==   defineList.at(j)->code()){
                        dItem[i].cnName = defineList.at(j)->name();
                        dItem[i].title = defineList.at(j)->name();
                        dItem[i].visible = defineList.at(j)->isView();

                    }
                }
            }
        }
    }

    QList<SupplieBean *> venBeanList = SupplieDAO::querySupplie();
    QStringList venList ;
    for(int i =0 ; i < venBeanList.size(); i++){
        venList << venBeanList.at(i)->supplieCode() + " - " +venBeanList.at(i)->supplieName();
    }
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
    QString sql = " SELECT a.id, a.no, a.bus_date, a.bus_type, a.supplier_id, b.supplie_name, a.wh_code,a.dep_code,a.demand_date,c.product_code,"
            " d.product_name, c.product_style, c.unit,  a.project_date, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
            " a.closer, a.updater, a.updated, a.status, c.quantity,c.not_quantity , c.price,c.money, "
            " c.memo, c.define1, c.define2, c.define3, c.define4, c.define5, c.define6 "
            " from pu_order_vouch a, "
            "( SELECT g.*,g.quantity-g.in_quantity- COALESCE(h.sum_qunatity,0) as not_quantity "
                " from  pu_order_vouchs g LEFT JOIN ( "
                " SELECT e.order_no,e.product_code,sum(quantity) as sum_qunatity  from pu_arrival_vouchs e inner join pu_arrival_vouch f ON e.arrival_no=f.no where f.is_handle=false AND f.bus_type='01'"
                " GROUP BY e.order_no,e.product_code) h "
                " ON g.order_no=h.order_no AND g.product_code=h.product_code "
            " where (g.is_complete=FALSE OR g.is_complete is null) AND g.quantity-g.in_quantity-COALESCE(h.sum_qunatity,0) > 0 "
            " ) c,ba_product d,ba_suppliers b "
            " WHERE a.bus_date>=? AND a.bus_date<=? AND b.supplie_type like ? AND a.supplier_id like ? "
            " AND d.type_id like ? AND c.product_code like ?"
            " AND a.no=c.order_no AND a.is_handle=true AND c.product_code=d.product_code AND a.supplier_id= b.supplie_code "
            " ORDER BY id ASC";
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
            SystemSetBean *useDefineBean = SystemSetDAO::getBean("101");

            foreach (int row, selectedRowsList){
                cusRow = ui->tableWidget->rowCount() -1 ;
                QString orderNo = modelChoose->data(modelChoose->index(row,0)).toString();
                QString productCode = modelChoose->data(modelChoose->index(row,5)).toString();
                double quantity = modelChoose->data(modelChoose->index(row,10)).toDouble();
                double price = modelChoose->data(modelChoose->index(row,11)).toDouble();
                double money = quantity * price;
                WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"productCode",productCode);
                productCode = ui->tableWidget->model()->data(ui->tableWidget->model()->index(cusRow,WidgetBeanUtil::getChildItemId(mChildItem,"productCode"))).toString();
                if(productCode.trimmed().length() > 1){
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"price",price);
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"quantity",quantity);
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"money",money);
                    WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"orderNo",orderNo);
                }

                if(useDefineBean && useDefineBean->id().length() > 0){
                    if(useDefineBean->yesNo()){
                        QString memo = modelChoose->data(modelChoose->index(row,19)).toString();
                        QString define1 = modelChoose->data(modelChoose->index(row,13)).toString();
                        QString define2 = modelChoose->data(modelChoose->index(row,14)).toString();
                        QString define3 = modelChoose->data(modelChoose->index(row,15)).toString();
                        QString define4 = modelChoose->data(modelChoose->index(row,16)).toString();
                        QString define5 = modelChoose->data(modelChoose->index(row,17)).toString();
                        QString define6 = modelChoose->data(modelChoose->index(row,18)).toString();
                        WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"define1",define1);
                        WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"define2",define2);
                        WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"define3",define3);
                        WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"define4",define4);
                        WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"define5",define5);
                        WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"define6",define6);
                        WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,cusRow,mChildItem,"memo",memo);
                    }
                }

            }
            mIsChoose = false;



        }

    }
}

void PuArrivalWin::setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage){
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


void PuArrivalWin::on_tableWidget_itemChanged(QTableWidgetItem *item)
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

        //�ɹ�ҵ����ж�������ֱ��¼����Ʒ����
        if(mIsHathOrder && !mIsChoose){
            QMessageBox::warning(this,"��ʾ","�˲ɹ�ҵ����ж���������ֱ��¼����Ʒ��");
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
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"quantity",1);
            WidgetBeanUtil::setValueToTableWidget(ui->tableWidget,item->row(),mChildItem,"money",1*bean->puPrice());

            if((ui->tableWidget->rowCount()-1) == item->row()){
                QAbstractItemModel *model = ui->tableWidget->model();
                int id = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"id"))).toInt();
                if(mEditStatus == editFlag && id < 1){

                    //�����޸� ��������
                    PuArrivalVouchsBean *puVouchsBean = new PuArrivalVouchsBean();
                    puVouchsBean->setArrivalNo(ui->noEdit->text());
                    WidgetBeanUtil::setItemModelToBean(model,item->row(),mChildItem,puVouchsBean);
                    puVouchsBean->setTaxPrice(puVouchsBean->price());
                    mAddChildVouchsList.append(puVouchsBean);
                }
                ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            }
        }else{
            QMessageBox::information(this,"��ʾ","�����ڴ���Ʒ");

            return ;
        }

    }else if(item->column() == 4 || item->column()==5){

        QTableWidgetItem *moneyItemR = new QTableWidgetItem();

        double price = ui->tableWidget->model()->data(ui->tableWidget->model()->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"price"))).toDouble();
        double quantity = ui->tableWidget->model()->data(ui->tableWidget->model()->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"quantity"))).toDouble();
        double money = price * quantity;
        moneyItemR->setData(Qt::EditRole,QString::number(money,'f',2));
        ui->tableWidget->setItem(item->row(),6,moneyItemR);
        if(mIsEdit){
            mTotalQuantity = 0;
            mTotalSum = 0;
            QAbstractItemModel *model = ui->tableWidget->model();
            for(int i = 0 ; i < ui->tableWidget->rowCount(); i++){
                double quantity = model->data(model->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"quantity"))).toDouble();
                double money = model->data(model->index(i,WidgetBeanUtil::getChildItemId(mChildItem,"money"))).toDouble();

                mTotalQuantity = mTotalQuantity + quantity;
                mTotalSum = mTotalSum + money;

            }
            mFootModel->setItem(5,new QStandardItem(QString::number(mTotalQuantity,'f',2)));
            mFootModel->setItem(7,new QStandardItem(QString::number(mTotalSum,'f',2)));
            mFootView->reset();

        }

    }

    //�����޸�
    if(mEditStatus == editFlag){
        QAbstractItemModel *model = ui->tableWidget->model();
        int id = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"id"))).toInt();
        QString pCode = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"productCode"))).toString();
        double price = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"price"))).toDouble();
        double quantity = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"quantity"))).toDouble();
        QString orderNo = model->data(model->index(item->row(),WidgetBeanUtil::getChildItemId(mChildItem,"orderNo"))).toString();

        if(item->column() == 4 || item->column()==5 || item->column()==8){
            if(id < 1 ){
                for(int i =0; i < mAddChildVouchsList.size(); i++){
                    if(mAddChildVouchsList.at(i)->productCode() == pCode){
                        double money = price * quantity;
                        mAddChildVouchsList.at(i)->setQuantity(quantity);
                        mAddChildVouchsList.at(i)->setPrice(price);
                        mAddChildVouchsList.at(i)->setTaxPrice(price);
                        mAddChildVouchsList.at(i)->setMoney(money);
                        mAddChildVouchsList.at(i)->setOrderNo(orderNo);
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

                mChildVouchsList.at(i)->setMoney(money);

            }
        }

    }
}
