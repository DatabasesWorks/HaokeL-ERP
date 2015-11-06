#include "rdoutimeiquery.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "bean/CustomerBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "dao/customerdao.h"
#include "bc/rdoutimeiwin.h"
#include "haokeapp.h"
#include "bcplugin.h"

RdOutImeiQuery::RdOutImeiQuery(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 =
            " SELECT a.*,b.product_name,c.cus_name,d.wh_name "
            " FROM rd_out_imei a "
            " INNER JOIN ba_product b ON a.product_code=b.product_code "
            " LEFT JOIN ba_customer c ON a.bus_code=c.cus_code "
            " LEFT JOIN ba_ware_house d ON a.wh_code=d.wh_code "
            " WHERE (vouch_type='02' OR vouch_type='11') AND bus_date>=? AND bus_date<=? "
            " AND a.bus_code LIKE ? AND b.type_id LIKE ? AND a.product_code LIKE ?";
    QString dSql2 = " SELECT a.*,b.product_name,c.cus_name,d.wh_name "
            " FROM rd_out_imei a "
            " INNER JOIN ba_product b ON a.product_code=b.product_code "
            " LEFT JOIN ba_customer c ON a.bus_code=c.cus_code "
            " LEFT JOIN ba_ware_house d ON a.wh_code=d.wh_code "
            " WHERE imei LIKE ? ";

    childItemProperties childItem[] = {
            { "no","����","����", 0,100, true, false },
            { "imei","IMEI","IMEI", 1,120, true, true },
            { "bus_code","����","�ͻ�", 2, 80,true, true },
            { "cus_name","����","�ͻ�", 3, 160,true, true },
            { "product_code","��Ʒ����","��Ʒ����", 4,80, true, true },
            { "product_name","��Ʒ����","��Ʒ����", 5,80, true, true },
            { "wh_name","�ֿ�","�ֿ�", 6,100, true, true },
            { "bus_date","��������","��������", 7,100, true, true },
            { "memo","��ע","��ע",8,180, true, false }
    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    QList<CustomerBean *> cusBeanList = CustomerDAO::queryCus();
    QStringList cusList ;
    for(int i =0 ; i < cusBeanList.size(); i++){
        cusList << cusBeanList.at(i)->cusCode() + " - " +cusBeanList.at(i)->cusName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(cusList);
    dwTypeLabel->setText("��    ��:");
    dwNameQks->setVisible(false);
    dwNameLabel->setVisible(false);

    Label->setVisible(false);
    //idLabel->setVisible(false);
    //noEdit->setVisible(false);
    idLabel->setText("��    ��:");
    Label->setVisible(false);
    rBnt1->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    //verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);
    //gridLayout->addItem(verticalSpacer_2, 5, 1, 1, 1);
    QString formId = HaoKeApp::bcSubId() + BcPlugin::rdInImeiQueryWinId();
    setFormId(formId);
    setTabDetailText("���⴮��");

    initData(dItem,dSql1,dSql2);
    initTableHead();
    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

RdOutImeiQuery::RdOutImeiQuery(QString vouch_type, QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 =
            " SELECT a.*,b.product_name,c.cus_name,d.wh_name "
            " FROM rd_out_imei a "
            " INNER JOIN ba_product b ON a.product_code=b.product_code "
            " LEFT JOIN ba_customer c ON a.bus_code=c.cus_code "
            " LEFT JOIN ba_ware_house d ON a.wh_code=d.wh_code "
            " WHERE (vouch_type='02' OR vouch_type='11') AND bus_date>=? AND bus_date<=? "
            " AND a.bus_code LIKE ? AND b.type_id LIKE ? AND a.product_code LIKE ?";
    QString dSql2 = " SELECT a.*,b.product_name,c.cus_name,d.wh_name "
            " FROM rd_out_imei a "
            " INNER JOIN ba_product b ON a.product_code=b.product_code "
            " LEFT JOIN ba_customer c ON a.bus_code=c.cus_code "
            " LEFT JOIN ba_ware_house d ON a.wh_code=d.wh_code "
            " WHERE imei LIKE ? ";
    QList<struct childItemProperties > dItem;

    if(vouch_type == HaoKeApp::getVouchRdPuRe() ){

        dSql1 = " SELECT a.*,b.product_name,c.supplie_name,d.wh_name "
                " FROM rd_out_imei a "
                " INNER JOIN ba_product b ON a.product_code=b.product_code "
                " LEFT JOIN ba_suppliers c ON a.bus_code=c.supplie_code "
                " LEFT JOIN ba_ware_house d ON a.wh_code=d.wh_code "
                " WHERE vouch_type='14' AND bus_date>=? AND bus_date<=? "
                " AND a.bus_code LIKE ? AND b.type_id LIKE ? AND a.product_code LIKE ?";

        childItemProperties childItem[] = {
                    { "no","����","����", 0,100, true, false },
                    { "imei","IMEI","IMEI", 1,120, true, true },
                    { "bus_code","����","��Ӧ��", 2, 80,true, true },
                    { "supplie_name","����","��Ӧ��", 3, 160,true, true },
                    { "product_code","��Ʒ����","��Ʒ����", 4,80, true, true },
                    { "product_name","��Ʒ����","��Ʒ����", 5,80, true, true },
                    { "wh_name","�ֿ�","�ֿ�", 6,100, true, true },
                    { "bus_date","�˿�����","�˿�����", 7,100, true, true },
                    { "memo","��ע","��ע",8,180, true, false }
        };
        int size = sizeof(childItem) / sizeof(childItem[0]);
        for(int i = 0 ; i < size ; i++){
            dItem << childItem[i];
        }
        QList<SupplieBean *> venBeanList = SupplieDAO::querySupplie();
        QStringList venList ;
        for(int i =0 ; i < venBeanList.size(); i++){
            venList << venBeanList.at(i)->supplieCode() + " - " +venBeanList.at(i)->supplieName();
        }
        dwTypeQks->setListModel(true);
        dwTypeQks->setItems(venList);
        dwTypeLabel->setText("�� Ӧ ��:");
        setTabDetailText("�ɹ��˻�����");
        QString formId = HaoKeApp::bcSubId() + BcPlugin::rdReInImeiQueryWinId();
        setFormId(formId);

    }else{
        childItemProperties childItem[] = {
                { "no","����","����", 0,100, true, false },
                { "imei","IMEI","IMEI", 1,120, true, true },
                { "bus_code","����","�ͻ�", 2, 80,true, true },
                { "cus_name","����","�ͻ�", 3, 160,true, true },
                { "product_code","��Ʒ����","��Ʒ����", 4,80, true, true },
                { "product_name","��Ʒ����","��Ʒ����", 5,80, true, true },
                { "wh_name","�ֿ�","�ֿ�", 6,100, true, true },
                { "bus_date","�������","�������", 7,100, true, true },
                { "memo","��ע","��ע",8,180, true, false }
        };
        int size = sizeof(childItem) / sizeof(childItem[0]);
        for(int i = 0 ; i < size ; i++){
            dItem << childItem[i];
        }
        QList<CustomerBean *> cusBeanList = CustomerDAO::queryCus();
        QStringList cusList ;
        for(int i =0 ; i < cusBeanList.size(); i++){
            cusList << cusBeanList.at(i)->cusCode() + " - " +cusBeanList.at(i)->cusName();
        }
        dwTypeQks->setListModel(true);
        dwTypeQks->setItems(cusList);

        dwTypeLabel->setText("��    ��:");
        setTabDetailText("���⴮��");
        QString formId = HaoKeApp::bcSubId() + BcPlugin::rdInImeiQueryWinId();
        setFormId(formId);
    }

    dwNameQks->setVisible(false);
    dwNameLabel->setVisible(false);

    Label->setVisible(false);
    //idLabel->setVisible(false);
    //noEdit->setVisible(false);
    idLabel->setText("��    ��:");
    Label->setVisible(false);
    rBnt1->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    //verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);
    //gridLayout->addItem(verticalSpacer_2, 5, 1, 1, 1);

    initData(dItem,dSql1,dSql2);
    initTableHead();

    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void RdOutImeiQuery::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen("�������")){
            RdOutImeiWin *pWin = new RdOutImeiWin(no,this);
            pWin->setWindowTitle("�������");
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}


