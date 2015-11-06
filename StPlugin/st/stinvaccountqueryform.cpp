#include "stinvaccountqueryform.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "bean/WareHouseBean.h"
#include "dao/waredao.h"
#include "dao/rddao.h"
#include "stinwin.h"
#include "stoutwin.h"
#include "strdwin.h"
#include "haokeapp.h"
#include "stplugin.h"

StInvAccountQueryForm::StInvAccountQueryForm(QWidget *parent) :
    AnyQueryForm(parent)
{
    QString dSql1 = " SELECT t.*,end_q-in_q+out_q as start_q FROM st_inv_account(?, ?, ?, ?, true) "
            " t(row_sum bigint , wh_name varchar , mx_id integer ,bus_date date,rd_no varchar,name varchar, "
            " wh_code varchar,cus_code varchar,product_code varchar,in_q numeric,out_q numeric,end_q integer,product_name varchar) ";
    QString dSql2 = "";
    childItemProperties childItem[] = {
            { "rd_no","����","����", 0,100, true, true },
            { "bus_date","����","����", 1,100, true, true },
            { "wh_code","����","�ֿ�", 2,60, true, true },
            { "wh_name","����","�ֿ�", 3,80, true, true },
            { "name","��������","��������", 4,80, true, true },
            { "product_code","����","��Ʒ", 5,80, true, true },
            { "product_name","����","��Ʒ", 6,100, true, true },
            { "start_q","�ڳ���","�ڳ���", 7, 80,true, true },
            { "in_q","��������","��������", 8, 80,true, true },
            { "out_q","��������","��������", 9, 80,true, true },
            { "end_q","�������","�������", 10,80, true, true }
    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }
    /*
    QList<CustomerBean *> venBeanList = CustomerDAO::queryCus();
    QStringList venList ;
    for(int i =0 ; i < venBeanList.size(); i++){
        venList << venBeanList.at(i)->cusCode() + " - " +venBeanList.at(i)->cusName();
    }
    dwNameQks->setListModel(true);
    dwNameQks->setItems(venList);
    */
    QDate oneDate = QDate(HaoKeApp::getInstance()->getOperateDate().year(),HaoKeApp::getInstance()->getOperateDate().month(),1);
    sDateEdit1->setDate(oneDate);
    dwNameQks->setVisible(false);
    dwNameLabel->setVisible(false);
    noEdit->setVisible(false);
    Label->setVisible(false);
    //idLabel->setVisible(false);
    QString note =  "<html><head/><body><p><span style=' font-weight:600; color:#ff0000;'> %1 </span></p></body></html>";

    idLabel->setText(note.arg("��Ʒ̨�ʲ�ѯ����ѡ��ֿ�����Ʒ"));
    QList<WareHouseBean *> beanList = WareDAO::queryWarehouse();
    QStringList typeList ;
    for(int i =0 ; i < beanList.size(); i++){
        typeList << beanList.at(i)->whCode() + " - " +beanList.at(i)->whName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(typeList);
    dwTypeLabel->setText("��    ��");
    proTypeLabel->setVisible(false);
    proTypeQks->setVisible(false);
    Label->setVisible(false);
    rBnt1->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    QString formId = HaoKeApp::stSubId() + StPlugin::invAccountQueryWinId();
    QString winTitle = "��Ʒ̨��";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::saSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabDetailText("��Ʒ̨��");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 8 << 9;
    setTotalItem(totalItem);
    initData(dItem,dSql1,dSql2);
    initTableHead();


    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void StInvAccountQueryForm::sTableClicked(QString no){
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
