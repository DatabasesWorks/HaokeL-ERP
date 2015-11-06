#include "stjournalaccountqueryform.h"
#include "bean/CustomerBean.h"
#include "bean/CustomerTypeBean.h"
#include "bean/WareHouseBean.h"
#include "dao/customerdao.h"
#include "dao/customertypedao.h"
#include "dao/waredao.h"
#include "dao/rddao.h"
#include "stoutwin.h"
#include "stinwin.h"
#include "strdwin.h"
#include "haokeapp.h"
#include "stplugin.h"
StJournalAccountQueryForm::StJournalAccountQueryForm(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 = " SELECT t.* FROM st_journal_account(?, ?, ?, ?, ?, true) "
            " t(mx_id integer ,bus_date date,rd_no varchar,bus_no varchar,bus_type varchar, wh_code varchar, "
            " cus_code varchar,product_code varchar, "
            " product_style varchar,handler varchar,is_handle bool, audit_time timestamp, created_by bigint, "
            " creater varchar, in_q numeric,in_price numeric,in_money numeric,out_q numeric,out_price numeric,"
            " out_money numeric,wh_name varchar,product_name varchar) ";
    QString dSql2 = "";
    childItemProperties childItem[] = {
        { "rd_no","����","����", 0,100, true, true },
        { "bus_date","����","����", 1,80, true, true },
        { "bus_no","Դ����","Դ����", 2,100, true, true },
        { "wh_code","����","�ֿ�", 3,60, true, true },
        { "wh_name","����","�ֿ�", 4,80, true, true },
        { "bus_type","��������","��������", 5,80, true, true },
        { "is_handle","�Ƿ����","�Ƿ����", 6,60, true, true },
        { "handler","�����","�����", 7,60, true, true },
        { "audit_time","���ʱ��","���ʱ��", 8,120, true, true },
        { "creater","�Ƶ���","�Ƶ���", 9,60, true, true },
        { "product_name","����","��Ʒ", 10,80, true, true },
        { "product_style","���","��Ʒ", 11,80, true, true },
        { "in_q","�������","�������", 12, 60,true, true },
        { "in_price","��ⵥ��","��ⵥ��", 13, 60,true, true },
        { "in_money","�����","�����", 14, 80,true, true },
        { "out_q","��������","��������", 15, 60,true, true },
        { "out_price","���ⵥ��","���ⵥ��", 16, 60,true, true },
        { "out_money","������","������", 17, 80,true, true },
    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }
    QDate oneDate = QDate(HaoKeApp::getInstance()->getOperateDate().year(),HaoKeApp::getInstance()->getOperateDate().month(),1);
    sDateEdit1->setDate(oneDate);
    /*
    QList<CustomerBean *> venBeanList = CustomerDAO::queryCus();
    QStringList venList ;
    for(int i =0 ; i < venBeanList.size(); i++){
        venList << venBeanList.at(i)->cusCode() + " - " +venBeanList.at(i)->cusName();
    }
    dwNameQks->setListModel(true);
    dwNameQks->setItems(venList);
    */
    dwNameQks->setVisible(false);
    dwNameLabel->setVisible(false);
    noEdit->setVisible(false);
    Label->setVisible(false);
    idLabel->setVisible(false);
    QList<WareHouseBean *> beanList = WareDAO::queryWarehouse();
    QStringList typeList ;
    for(int i =0 ; i < beanList.size(); i++){
        typeList << beanList.at(i)->whCode() + " - " +beanList.at(i)->whName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(typeList);
    dwTypeLabel->setText("��    ��");

    Label->setVisible(false);
    rBnt1->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    QString formId = HaoKeApp::stSubId() + StPlugin::stockQueryWinId();
    QString winTitle = "�������ˮ��";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::saSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabDetailText("�������ˮ��");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 12 << 14 << 15 << 17;
    setTotalItem(totalItem);
    initData(dItem,dSql1,dSql2);
    initTableHead();


    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void StJournalAccountQueryForm::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen("����ⵥ")){
            RdBean *bean = RdDAO::getRdByNo(no);
            if(bean && bean->rdNo().length() > 1){
                if(bean->vouchType() == HaoKeApp::getVouchRdOtherIn()){
                    StInWin *pWin = new StInWin(no,this);
                    pWin->setWindowTitle("����ⵥ");
                    HaoKeApp::getInstance()->gui()->addSubWin(pWin);
                    pWin->showMaximized();
                }else if(bean->vouchType() == HaoKeApp::getVouchRdOtherOut()){
                    StOutWin *pWin = new StOutWin(no,this);
                    pWin->setWindowTitle("����ⵥ");
                    HaoKeApp::getInstance()->gui()->addSubWin(pWin);
                    pWin->showMaximized();
                }else{
                    StRdWin *pWin = new StRdWin(no,this);
                    pWin->setWindowTitle("����ⵥ");
                    HaoKeApp::getInstance()->gui()->addSubWin(pWin);
                    pWin->showMaximized();
                }
            }
        }
    }
}
