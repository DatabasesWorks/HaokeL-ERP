#include "fatransceiverssummaryqueryform.h"
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
FaTransceiversSummaryQueryForm::FaTransceiversSummaryQueryForm(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 = " SELECT t.* FROM fa_transceivers_summary(?, ?, ?, ?, ?) "
            " t(product_code varchar , be_quantity numeric , be_cost numeric , be_money numeric , "
            " in_quantity numeric , in_cost numeric , in_money numeric ,"
            " out_quantity numeric , out_cost numeric , out_money numeric ,"
            " st_quantity numeric , st_cost numeric , st_money numeric ,"
            " product_name varchar, style varchar, unit varchar) ";

    QString dSql2 = "";
    childItemProperties childItem[] = {
            { "product_code","����","��Ʒ", 0,80, true, true },
            { "product_name","����","��Ʒ", 1,100, true, true },
            { "style","���","��Ʒ",2,80, true, false },
            { "unit","��λ","��λ", 3,60, true, false },
            { "be_quantity","����","�ڳ�", 4, 80,true, true },
            { "be_cost","����","�ڳ�", 5, 80,true, true },
            { "be_money","���","�ڳ�", 6, 80,true, true },
            { "in_quantity","����","���", 7,80, true, true },
            { "in_cost","����","���", 8,80, true, true },
            { "in_money","���","���", 9,80, true, true },
            { "out_quantity","����","����", 10,80, true, true },
            { "out_cost","����","����", 11,80, true, true },
            { "out_money","���","����", 12,80, true, true },
            { "st_quantity","����","��ĩ", 13,80, true, true },
            { "st_cost","����","��ĩ", 14,80, true, true },
            { "st_money","���","��ĩ", 15,80, true, true }
    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    /*
    QList<DepBean *> depBeanList = DepDAO::queryDep();
    QStringList depList ;
    for(int i =0 ; i < depBeanList.size(); i++){
        depList << depBeanList.at(i)->depId() + " - " +depBeanList.at(i)->depName();
    }
    dwNameQks->setListModel(true);
    dwNameQks->setItems(depList);
    dwNameLabel->setText("��    ��:");
    */
    dwNameQks->setVisible(false);
    dwNameLabel->setVisible(false);
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
    Label->setVisible(false);
    rBnt1->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    QString formId = HaoKeApp::faSubId() + WFaPlugin::faTransceiversSummaryWinId();
    QString winTitle = "�ջ�����ܱ�";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::faSubId().toInt(),formId.toInt(),0,"");
    QDate oneDate = QDate(HaoKeApp::getInstance()->getOperateDate().year(),HaoKeApp::getInstance()->getOperateDate().month(),1);
    sDateEdit1->setDate(oneDate);
    setFormId(formId);
    setTabDetailText("�ջ�����ܱ�");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 4 << 6 << 7 << 9 << 10 << 12 << 13 << 15;
    setTotalItem(totalItem);
    initData(dItem,dSql1,dSql2);
    initTableHead();

    //�趨�Ƿ�������״̬��ѯ
    //setHandleFiled("c.is_complete");

    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void FaTransceiversSummaryQueryForm::sTableClicked(QString no){

}
