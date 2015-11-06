#include "allocationqueryform.h"
#include "bean/WareHouseBean.h"
#include "bean/TransVouchBean.h"
#include "bean/TransVouchsBean.h"
#include "dao/waredao.h"
#include "allocationwin.h"
#include "haokeapp.h"
#include "stplugin.h"
AllocationQueryForm::AllocationQueryForm(QWidget *parent) :
    QueryForm(parent)
{
    QString lSql1 = " select f.*,b.wh_name as owh_name,b1.wh_name as iwh_name from"
            " (select a.*,c.dep_name,d.emp_name from st_trans_vouch a"
                " LEFT JOIN ba_department c ON a.dep_code=c.dep_id"
                " LEFT JOIN ba_employee d ON a.person_code=d.emp_code"
                " WHERE a.tv_date>=? AND a.tv_date<=? AND a.owh_code LIKE ? AND a.iwh_code LIKE ?  ) f"
                " INNER JOIN ba_ware_house b ON f.owh_code=b.wh_code"
                " INNER JOIN ba_ware_house b1 ON f.iwh_code=b1.wh_code"
            " ORDER BY f.tv_date,f.tv_no ASC";

    QString lSql2 = " select f.*,b.wh_name as owh_name,b1.wh_name as iwh_name from"
            " (select a.*,c.dep_name,d.emp_name from st_trans_vouch a"
                " LEFT JOIN ba_department c ON a.dep_code=c.dep_id"
                " LEFT JOIN ba_employee d ON a.person_code=d.emp_code"
                " WHERE a.tv_no like ? ) f"
                " INNER JOIN ba_ware_house b ON f.owh_code=b.wh_code"
                " INNER JOIN ba_ware_house b1 ON f.iwh_code=b1.wh_code";
    childItemProperties listItem[] = {
            { "tv_no","����","����", 0,100, true, true },
            { "tv_date","��������","��������", 1,80, true, true },
            { "owh_name","ת���ֿ�","ת���ֿ�", 2,80, true, false },
            { "iwh_name","ת��ֿ�","ת��ֿ�", 3,80, true, false },
            { "dep_name","����","����", 4,80, true, false },
            { "emp_name","ҵ��Ա","ҵ��Ա", 5,60, true, false },
            { "handler","���","���", 6,60, true, false },
            { "audit_time","���ʱ��","���ʱ��", 7,120, true, false },
            { "creater","�Ƶ�","�Ƶ�", 8,60, true, false },
            { "memo","��ע","��ע", 9,180, true, false },

    };
    QString dSql1 =
            " select f.*,b.wh_name as owh_name,b1.wh_name as iwh_name,p.product_name from"
                        " (select a.*,c.dep_name,d.emp_name,e.product_code,e.product_style,e.unit, "
                            " e.quantity,e.unit_price,e.money "
                            " from st_trans_vouch a"
                            " LEFT JOIN ba_department c ON a.dep_code=c.dep_id"
                            " LEFT JOIN ba_employee d ON a.person_code=d.emp_code"
                            " INNER JOIN st_trans_vouchs e "
                            " ON a.tv_no=e.tv_no "
                            " WHERE a.tv_date>=? AND a.tv_date<=? AND a.owh_code LIKE ? AND a.iwh_code LIKE ? ) f"
                            " INNER JOIN ba_ware_house b ON f.owh_code=b.wh_code"
                            " INNER JOIN ba_ware_house b1 ON f.iwh_code=b1.wh_code"
                            " INNER JOIN ba_product p ON f.product_code=p.product_code "
                            " WHERE p.type_id LIKE ? AND f.product_code LIKE ? "
                        " ORDER BY f.tv_date,f.tv_no ASC";
    QString dSql2 = " select f.*,b.wh_name as owh_name,b1.wh_name as iwh_name,p.product_name from"
            " (select a.*,c.dep_name,d.emp_name,e.product_code,e.product_style,e.unit, "
                " e.quantity,e.unit_price,e.money "
                " from st_trans_vouch a"
                " LEFT JOIN ba_department c ON a.dep_code=c.dep_id"
                " LEFT JOIN ba_employee d ON a.person_code=d.emp_code"
                " INNER JOIN st_trans_vouchs e "
                " ON a.tv_no=e.tv_no "
                " WHERE a.tv_no LIKE ? ) f"
                " INNER JOIN ba_ware_house b ON f.owh_code=b.wh_code"
                " INNER JOIN ba_ware_house b1 ON f.iwh_code=b1.wh_code"
                " INNER JOIN ba_product p ON f.product_code=p.product_code ";
    childItemProperties childItem[] = {
            { "tv_no","����","����", 0,100, true, true },
            { "tv_date","��������","��������", 1,80, true, true },
            { "owh_name","ת���ֿ�","ת���ֿ�", 2,80, true, false },
            { "iwh_name","ת��ֿ�","ת��ֿ�",3,80, true, true },
            { "product_code","����","��Ʒ",4,80, true, true },
            { "product_name","����","��Ʒ",5,100, true, true },
            { "product_style","���","��Ʒ",6,80, true, false },
            { "unit","��λ","��λ", 7,60, true, false },
            { "quantity","����","����", 8, 60,true, true },
            { "unit_price","����","����", 9,80, true, true },
            { "money","���","���", 10,80, true, false }

    };
    QList<struct childItemProperties > lItem;
    int size = sizeof(listItem) / sizeof(listItem[0]);
    for(int i = 0 ; i < size ; i++){
        lItem << listItem[i];
    }
    QList<struct childItemProperties > dItem;
    size = sizeof(childItem) / sizeof(childItem[0]);
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

    QList<CustomerTypeBean *> typeBeanList = CustomerTypeDAO::queryType();
    QStringList typeList ;
    for(int i =0 ; i < typeBeanList.size(); i++){
        typeList << typeBeanList.at(i)->typeId() + " - " +typeBeanList.at(i)->typeName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(typeList);
    */

    QList<WareHouseBean *> beanList = WareDAO::queryWarehouse();
    QStringList typeList ;
    for(int i =0 ; i < beanList.size(); i++){
        typeList << beanList.at(i)->whCode() + " - " +beanList.at(i)->whName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(typeList);
    dwTypeLabel->setText("ת���ֿ�");

    dwNameQks->setListModel(true);
    dwNameQks->setItems(typeList);
    dwNameLabel->setText("ת��ֿ�");
    QString formId = HaoKeApp::stSubId() + StPlugin::alloQueryWinId();
    QString winTitle = "������ѯ";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::stSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabListText("�������б�");
    setTabDetailText("������ϸ");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 8 << 10;
    setTotalItem(totalItem);
    initData(lItem,dItem,lSql1,lSql2,dSql1,dSql2);
    initTableHead();
    //�趨�Ƿ�������״̬��ѯ
    setHandleFiled("is_handle");
    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void AllocationQueryForm::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen(tr("Allocation Invoice"))){
            AllocationWin *pWin = new AllocationWin(no,this);
            pWin->setWindowTitle(tr("Allocation Invoice"));
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}
