#include "fasubsidiaryqueryform.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "bean/WareHouseBean.h"
#include "bean/DepBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "dao/waredao.h"
#include "dao/depdao.h"
#include "haokeapp.h"
#include "wfaplugin.h"
FaSubsidiaryQueryForm::FaSubsidiaryQueryForm(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 =
            " SELECT a.*, st_quantity-in_quantity+out_quantity as be_quantity,st_money-in_money+out_money as be_money,"
            " b.wh_name,c.product_name,d.dep_name,c.style FROM fa_subsidiary a "
            " INNER JOIN ba_ware_house b ON a.wh_code=b.wh_code "
            " INNER JOIN ba_product c ON a.product_code=c.product_code "
            " LEFT JOIN ba_department d ON a.dep_code=d.dep_id "
            " WHERE a.bus_date>=? AND a.bus_date<=? AND a.wh_code LIKE ? AND a.dep_code LIKE ? "
            " AND c.type_id LIKE ? AND a.product_code LIKE ?"
            " ORDER BY a.id ASC";
    QString dSql2 = "";
    childItemProperties childItem[] = {
            { "vouch_no","����","����", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "pz_digest","ժҪ","ժҪ", 2,80, true, false },
            { "bus_type","��������","��������", 3,60, true, false },
            { "wh_name","�ֿ�","�ֿ�", 4, 100,true, true },
            { "product_code","����","��Ʒ", 5,80, true, true },
            { "product_name","����","��Ʒ", 6,100, true, true },
            { "style","���","��Ʒ",7,80, true, false },
            { "be_quantity","����","�ڳ�", 8, 60,true, true },
            { "be_money","���","�ڳ�", 9, 80,true, true },
            { "in_quantity","����","����", 10, 60,true, true },
            { "in_cost","����","����", 11,60, true, true },
            { "in_money","���","����", 12,80, true, true },
            { "out_quantity","����","����", 13,60, true, false },
            { "out_cost","����","����", 14,60, true, false },
            { "out_money","���","����", 15,80, true, false },
            { "st_quantity","����","���", 16, 60,true, true },
            { "st_money","���","���", 17, 80,true, true },


    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    QList<DepBean *> depBeanList = DepDAO::queryDep();
    QStringList depList ;
    for(int i =0 ; i < depBeanList.size(); i++){
        depList << depBeanList.at(i)->depId() + " - " +depBeanList.at(i)->depName();
    }
    dwNameQks->setListModel(true);
    dwNameQks->setItems(depList);
    dwNameLabel->setText("��    ��:");
    QList<WareHouseBean *> whBeanList = WareDAO::queryWarehouse();
    QStringList whList ;
    for(int i =0 ; i < whBeanList.size(); i++){
        whList << whBeanList.at(i)->whCode() + " - " +whBeanList.at(i)->whName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(whList);
    dwTypeLabel->setText("��    ��:");
    idLabel->setVisible(false);
    noEdit->setVisible(false);
    rBnt1->setVisible(false);
    Label->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    QString formId = HaoKeApp::faSubId() + WFaPlugin::faSubsidiaryWinId();
    QString winTitle = "�����ϸ��";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::faSubId().toInt(),formId.toInt(),0,"");
    QDate oneDate = QDate(HaoKeApp::getInstance()->getOperateDate().year(),HaoKeApp::getInstance()->getOperateDate().month(),1);
    sDateEdit1->setDate(oneDate);
    setFormId(formId);
    setTabDetailText("�����ϸ��");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 10 << 12 << 13 << 15;
    setTotalItem(totalItem);
    initData(dItem,dSql1,dSql2);
    initTableHead();

    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void FaSubsidiaryQueryForm::sTableClicked(QString no){

}
